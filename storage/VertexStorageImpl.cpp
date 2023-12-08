//
// Created by goras on 10.11.2023.
//

#include "../vertex/VertexExceptions.h"
#include "../vertex/Vertex.h"
#include "../registry/lock/StorageLocker.h"
#include "VertexStorageImpl.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

VertexStorageImpl::VertexStorageImpl(std::pair<size_t, size_t> hashRange) : m_hashRange(hashRange) {
    load();

    m_terminating = false;
    m_dirty = false;
    m_usages = 0;

    // TODO notify
    m_timer = std::thread([&]() {
        while (!m_terminating) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            dump();
        }
    });
}

VertexStorageImpl::~VertexStorageImpl() {
    m_terminating = true;
    m_timer.join();

    dump();
    for (const auto &it: m_vertices) {
        delete it.second;
    }
}

Vertex *VertexStorageImpl::getVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) {
        throw VertexOperationException(id, VertexErrorCode::VertexNotFound);
    }
    return m_vertices[id];
}

Vertex *VertexStorageImpl::addVertex(const std::string &id) {
    auto locker = StorageLocker(this);
    auto it = m_vertices.find(id);
    if (it != m_vertices.end()) {
        throw VertexOperationException(id, VertexErrorCode::VertexAlreadyExists);
    }
    if (m_hasher(id) < m_hashRange.first || m_hasher(id) > m_hashRange.second) {
        throw std::exception("Id is not in hash range");
    }
    auto *vertex = new Vertex(id);
    m_vertices.insert({id, vertex});
    m_dirty = true;
    return vertex;
}

void VertexStorageImpl::deleteVertex(const std::string &id) {
    auto locker = StorageLocker(this);
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) {
        throw VertexOperationException(id, VertexErrorCode::VertexNotFound);
    }
    Vertex *temp = m_vertices[id];
    m_vertices.erase(id);
    delete temp;
    m_dirty = true;
}

void VertexStorageImpl::addConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                      bool reverse) {
    auto locker = StorageLocker(this);
    getVertex(id1)->connect(connName, id2, reverse);
    m_dirty = true;
}

void VertexStorageImpl::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                         bool reverse) {
    auto locker = StorageLocker(this);
    getVertex(id1)->disconnect(connName, id2, reverse);
    m_dirty = true;
}

std::vector<std::string> VertexStorageImpl::getAllIds() {
    std::vector<std::string> result;
    result.reserve(m_vertices.size());
    for (const auto &n: m_vertices) {
        result.push_back(n.first);
    }
    return result;
}

std::pair<size_t, size_t> VertexStorageImpl::getHashRange() {
    return m_hashRange;
}

void VertexStorageImpl::dump() {
    if (!m_dirty) {
        return;
    }
    while (true) {
        if (m_usages > 0) {
            continue;
        }
        m_mutex.lock();
        if (m_usages > 0) {
            m_mutex.unlock();
            continue;
        }
        break;
    }
    try {
        fs::path data_dir("data");
        if (!fs::exists(data_dir)) {
            fs::create_directories(data_dir);
        }
        fs::path filepath =
                data_dir / (std::to_string(m_hashRange.first) + "-" + std::to_string(m_hashRange.second) + ".txt");
        std::ofstream outFile(filepath);
        if (!outFile.is_open()) {
            throw std::exception(("Failed to open data file: " + filepath.string()).c_str());
        }
        outFile << m_vertices.size() << "\n";
        for (const auto &n: m_vertices) {
            outFile << n.second->toString() << "\n";
        }
        if (outFile.fail()) {
            throw std::exception(("Failed to write data file: " + filepath.string()).c_str());
        }
        m_dirty = false;
        m_mutex.unlock();
    }
    catch (std::exception &) {
        m_mutex.unlock();
        throw;
    }

}

void VertexStorageImpl::load() {
    fs::path data_dir("data");
    if (!fs::exists(data_dir)) {
        fs::create_directories(data_dir);
    }
    fs::path filepath =
            data_dir / (std::to_string(m_hashRange.first) + "-" + std::to_string(m_hashRange.second) + ".txt");
    if (!fs::exists(filepath)) {
        return;
    }
    m_vertices.clear();
    std::ifstream inFile(filepath);
    int numVertices;
    std::string VertexString;
    inFile >> numVertices;
    std::getline(inFile, VertexString);
    for (int i = 0; i < numVertices; i++) {
        std::getline(inFile, VertexString);
        auto *vertex = new Vertex("");
        vertex->fillFromString(VertexString);
        m_vertices.insert({vertex->getId(), vertex});
    }
}

Vertex VertexStorageImpl::createBackup(const std::string &id) {
    return {Vertex(*getVertex(id))};
}

void VertexStorageImpl::restoreBackup(const Vertex &vertex) {
    auto locker = StorageLocker(this);
    if (m_vertices.find(vertex.getId()) != m_vertices.end()) {
        delete m_vertices[vertex.getId()];
        m_vertices.erase(vertex.getId());
    }
    auto *newVertex = new Vertex(vertex);
    m_vertices.insert({newVertex->getId(), newVertex});
    m_dirty = true;
}

void VertexStorageImpl::lock() {
    m_mutex.lock();
    m_usages++;
    m_mutex.unlock();
}

void VertexStorageImpl::unlock() {
    m_mutex.lock();
    m_usages = m_usages < 1 ? 0 : m_usages - 1;
    m_mutex.unlock();
}
