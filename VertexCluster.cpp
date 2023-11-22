//
// Created by goras on 10.11.2023.
//

#include "VertexExceptions.h"
#include "VertexCluster.h"
#include "Vertex.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

VertexCluster::VertexCluster(std::pair<size_t, size_t> hashRange) : m_hashRange(hashRange) {
    load();
}

VertexCluster::~VertexCluster() {
    dump();
    for (const auto &it: m_vertices) {
        delete it.second;
    }
}

Vertex *VertexCluster::getVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) {
        throw VertexOperationException(id);
    }
    return m_vertices[id];
}

Vertex *VertexCluster::addVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if (it != m_vertices.end()) {
        throw VertexOperationException(id);
    }
    if (m_hasher(id) < m_hashRange.first || m_hasher(id) > m_hashRange.second) {
        throw std::exception("Id is not in hash range");
    }
    auto *vertex = new Vertex(id);
    m_vertices.insert({id, vertex});
    return vertex;
}

void VertexCluster::deleteVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) {
        throw VertexOperationException(id);
    }
    Vertex *temp = m_vertices[id];
    m_vertices.erase(id);
    delete temp;
}

void VertexCluster::addConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                  bool reverse) {
    getVertex(id1)->connect(connName, id2, reverse);
}

void VertexCluster::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                     bool reverse) {
    getVertex(id1)->disconnect(connName, id2, reverse);
}

std::vector<std::string> VertexCluster::getAllIds() {
    std::vector<std::string> result;
    result.reserve(m_vertices.size());
    for (const auto &n: m_vertices) {
        result.push_back(n.first);
    }
    return result;
}

std::pair<size_t, size_t> VertexCluster::getHashRange() {
    return m_hashRange;
}

void VertexCluster::dump() const {
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
}

void VertexCluster::load() {
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

Vertex VertexCluster::createBackup(const std::string &id) {
    return {Vertex(*getVertex(id))};
}

void VertexCluster::restoreBackup(const Vertex &vertex) {
    Vertex *oldVertex = getVertex(vertex.getId());
    auto *newVertex = new Vertex(vertex);
    m_vertices.erase(vertex.getId());
    m_vertices.insert({newVertex->getId(), newVertex});
    delete oldVertex;
}
