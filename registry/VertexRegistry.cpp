//
// Created by goras on 10.11.2023.
//

#include "../vertex/VertexExceptions.h"
#include "../vertex/Vertex.h"
#include "../storage/LocalVertexStorage.h"
#include "../storage/RemoteVertexStorage.h"
#include "lock/VertexLocker.h"
#include "lock/DoubleVertexLocker.h"
#include "lock/StorageLocker.h"
#include "VertexRegistry.h"

#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

VertexRegistry::VertexRegistry() {
    fs::path config_dir("config");
    if (!fs::exists(config_dir)) {
        throw std::exception("Cannot find config dir");
    }
    fs::path filepath = config_dir / ("clusters.txt");
    if (!fs::exists(filepath)) {
        throw std::exception("Cannot find config file");
    }
    std::ifstream inFile(filepath);
    std::string s;
    int numRemoteStorages;
    inFile >> numRemoteStorages;
    std::getline(inFile, s);
    for (int i = 0; i < numRemoteStorages; i++) {
        std::string hostname;
        int port;
        int numConnections;
        inFile >> hostname >> port >> numConnections;
        std::getline(inFile, s);
        try {
            m_storages.push_back(new RemoteVertexStorage(hostname, port, numConnections));
        }
        catch (RemoteStorageError &e) {
            std::cout << "Cannot connect to " << hostname << ":" << port << ". Local storage will be used instead\n";
        }
    }
    m_fallbackStorage = new LocalVertexStorage({0, (size_t) -1});
}

VertexRegistry::~VertexRegistry() {
    std::vector<std::thread> threads;
    threads.reserve(m_storages.size() + 1);
    for (auto c: m_storages) {
        threads.emplace_back([=]() { delete c; });
    }
    for (auto c: m_invalidStorages) {
        threads.emplace_back([=]() { delete c; });
    }
    threads.emplace_back([=]() { delete m_fallbackStorage; });
    for (std::thread &t: threads) {
        t.join();
    }
}

VertexRegistry &VertexRegistry::getInstance() {
    static VertexRegistry instance;
    return instance;
}

Vertex VertexRegistry::getVertex(const std::string &id) {
    VertexLocker locker(id, m_mutex, m_lockedVertices);
    return getVertexNoLock(id);
}

Vertex VertexRegistry::addVertex(const std::string &id) {
    VertexLocker locker(id, m_mutex, m_lockedVertices);
    auto cluster = getClusterForId(id);
    try {
        return cluster->addVertex(id);
    }
    catch (RemoteStorageError &e) {
        processRemoteError(e);
        throw VertexOperationException(id, VertexNotFound);
    }
}

void VertexRegistry::deleteVertex(const std::string &id) {
    VertexLocker locker(id, m_mutex, m_lockedVertices);
    auto cluster = getClusterForId(id);
    try {
        const Vertex vertex = getVertexNoLock(id);
        if (!vertex.getInputConnections().empty() || !vertex.getOutputConnections().empty()) {
            throw VertexOperationException(vertex.getId(), VertexErrorCode::VertexHasConnections);
        }
        cluster->deleteVertex(id);
    }
    catch (RemoteStorageError &e) {
        processRemoteError(e);
        throw VertexOperationException(id, VertexNotFound);
    }
}

void
VertexRegistry::connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) {
    DoubleVertexLocker locker(id1, id2, m_mutex, m_lockedVertices);

    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);

    try {
        auto dumpLocker1 = StorageLocker(cluster1);
        auto dumpLocker2 = StorageLocker(cluster2);

        const Vertex vertex1 = cluster1->createBackup(id1);
        const Vertex vertex2 = cluster2->createBackup(id2);

        try {
            cluster1->addConnection(id1, connName, id2, false);
            cluster2->addConnection(id2, connName, id1, true);
        }
        catch (ConnectionOperationException &) {
            cluster1->restoreBackup(vertex1);
            cluster2->restoreBackup(vertex2);
            throw;
        }
    }
    catch (RemoteStorageError &e) {
        processRemoteError(e);
        throw ConnectionOperationException(id1, id2, connName, false, ConnectionNotFound);
    }
}

void
VertexRegistry::disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) {
    DoubleVertexLocker locker(id1, id2, m_mutex, m_lockedVertices);

    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);

    try {
        auto dumpLocker1 = StorageLocker(cluster1);
        auto dumpLocker2 = StorageLocker(cluster2);

        const Vertex vertex1 = cluster1->createBackup(id1);
        const Vertex vertex2 = cluster2->createBackup(id2);

        try {
            cluster1->removeConnection(id1, connName, id2, false);
            cluster2->removeConnection(id2, connName, id1, true);
        }
        catch (ConnectionOperationException &) {
            cluster1->restoreBackup(vertex1);
            cluster2->restoreBackup(vertex2);
            throw;
        }
    }
    catch (RemoteStorageError &e) {
        processRemoteError(e);
        throw ConnectionOperationException(id1, id2, connName, false, ConnectionNotFound);
    }
}

std::vector<std::string> VertexRegistry::getAllIds() {
    std::vector<std::string> result;
    try {
        for (auto c: m_storages) {
            auto ids = c->getAllIds();
            result.insert(result.end(), ids.begin(), ids.end());
        }
    }
    catch (RemoteStorageError &e) {
        processRemoteError(e);
    }
    return result;
}

Vertex VertexRegistry::getVertexNoLock(const std::string &id) {
    auto cluster = getClusterForId(id);
    try {
        return cluster->getVertex(id);
    }
    catch (RemoteStorageError &e) {
        processRemoteError(e);
        throw VertexOperationException(id, VertexNotFound);
    }
}

VertexStorage *VertexRegistry::getClusterForId(const std::string &id) {
    const size_t hash = m_hasher(id);
    for (auto c: m_storages) {
        auto range = c->getHashRange();
        if ((hash >= range.first) && (hash <= range.second)) {
            return c;
        }
    }
    return m_fallbackStorage;
}

void VertexRegistry::processRemoteError(RemoteStorageError &e) {
    auto *p = (VertexStorage *) e.storage;
    std::cout << "Storage server for hash range " << e.storage->getHashRange().first << " - "
              << e.storage->getHashRange().second
              << " is unavailable. Local storage will be used for this hash range\n";
    auto it = std::find(m_storages.begin(), m_storages.end(), p);
    if (it != m_storages.end()) {
        m_storages.erase(it);
        m_invalidStorages.insert(p);
    }
}
