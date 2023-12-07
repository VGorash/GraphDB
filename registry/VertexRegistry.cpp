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

namespace fs = std::filesystem;

VertexRegistry::VertexRegistry() {
    loadConfig();

    size_t clusterCapacity = ((size_t) -1) / m_numClusters;

//    m_clusters.reserve(m_numClusters);
//    size_t lowerBound = 0;
//    for (int i = 0; i < m_numClusters; i++) {
//        size_t upperBound = lowerBound + clusterCapacity - 1;
//        if (i == m_numClusters - 1) {
//            upperBound = (size_t) -1;
//        }
//        m_clusters.push_back(new LocalVertexStorage({lowerBound, upperBound}));
//        lowerBound = upperBound + 1;
//    }

    m_clusters.push_back(new RemoteVertexStorage("127.0.0.1", 2001, 5));
    m_clusters.push_back(new RemoteVertexStorage("127.0.0.1", 2002, 5));

}

VertexRegistry::~VertexRegistry() {
    std::vector<std::thread> threads;
    threads.reserve(m_clusters.size());
    for (auto c: m_clusters) {
        threads.emplace_back([=]() { delete c; });
    }
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
    if (!cluster) {
        throw std::exception("Cluster configuration error");
    }
    return cluster->addVertex(id);
}

void VertexRegistry::deleteVertex(const std::string &id) {
    VertexLocker locker(id, m_mutex, m_lockedVertices);
    auto cluster = getClusterForId(id);
    if (!cluster) {
        throw std::exception("Cluster configuration error");
    }
    const Vertex vertex = getVertexNoLock(id);
    if (!vertex.getInputConnections().empty() || !vertex.getOutputConnections().empty()) {
        throw VertexOperationException(vertex.getId(), VertexErrorCode::VertexHasConnections);
    }
    cluster->deleteVertex(id);
}

void
VertexRegistry::connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) {
    DoubleVertexLocker locker(id1, id2, m_mutex, m_lockedVertices);

    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);
    if (!cluster1 || !cluster2) {
        throw std::exception("Cluster configuration error");
    }

    auto dumpLocker1 = StorageLocker(cluster1);
    auto dumpLocker2 = StorageLocker(cluster2);

    const Vertex vertex1 = cluster1->createBackup(id1);
    const Vertex vertex2 = cluster2->createBackup(id2);

    try {
        cluster1->addConnection(id1, connName, id2, false);
        cluster2->addConnection(id2, connName, id1, true);
    }
    catch (std::exception &) {
        cluster1->restoreBackup(vertex1);
        cluster2->restoreBackup(vertex2);
        throw;
    }
}

void
VertexRegistry::disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) {
    DoubleVertexLocker locker(id1, id2, m_mutex, m_lockedVertices);

    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);
    if (!cluster1 || !cluster2) {
        throw std::exception("Cluster configuration error");
    }

    auto dumpLocker1 = StorageLocker(cluster1);
    auto dumpLocker2 = StorageLocker(cluster2);

    const Vertex vertex1 = cluster1->createBackup(id1);
    const Vertex vertex2 = cluster2->createBackup(id2);

    try {
        cluster1->removeConnection(id1, connName, id2, false);
        cluster2->removeConnection(id2, connName, id1, true);
    }
    catch (std::exception &) {
        cluster1->restoreBackup(vertex1);
        cluster2->restoreBackup(vertex2);
        throw;
    }
}

std::vector<std::string> VertexRegistry::getAllIds() {
    std::vector<std::string> result;
    for (auto c: m_clusters) {
        auto ids = c->getAllIds();
        result.insert(result.end(), ids.begin(), ids.end());
    }
    return result;
}

Vertex VertexRegistry::getVertexNoLock(const std::string &id) {
    auto cluster = getClusterForId(id);
    if (!cluster) {
        throw std::exception("Cluster configuration error");
    }
    return cluster->getVertex(id);
}

void VertexRegistry::loadConfig() {
    fs::path config_dir("config");
    if (!fs::exists(config_dir)) {
        throw std::exception("Cannot find config dir");
    }
    fs::path filepath = config_dir / ("clusters.txt");
    if (!fs::exists(filepath)) {
        throw std::exception("Cannot find config file");
    }
    std::ifstream inFile(filepath);
    inFile >> m_numClusters;
}

VertexStorage *VertexRegistry::getClusterForId(const std::string &id) {
    const size_t hash = m_hasher(id);
    for (auto c: m_clusters) {
        auto range = c->getHashRange();
        if (hash >= range.first && hash <= range.second) {
            return c;
        }
    }
    return nullptr;
}
