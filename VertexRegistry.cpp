//
// Created by goras on 10.11.2023.
//

#include "VertexRegistry.h"
#include "VertexCluster.h"
#include "Vertex.h"

#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

VertexRegistry::VertexRegistry() {
    loadConfig();

    size_t clusterCapacity = ((size_t)-1) / m_numClusters;

    m_clusters.reserve(m_numClusters);
    size_t lowerBound = 0;
    for(int i=0; i<m_numClusters; i++){
        size_t upperBound = lowerBound + clusterCapacity - 1;
        if(i == m_numClusters - 1) {
            upperBound = (size_t)-1;
        }
        m_clusters.push_back(new VertexCluster({lowerBound, upperBound}));
        lowerBound = upperBound + 1;
    }
}

VertexRegistry::~VertexRegistry() {
    for(auto c : m_clusters){
        c->dump();
        delete c;
    }
}

VertexRegistry &VertexRegistry::getInstance() {
    static VertexRegistry instance;
    return instance;
}

const Vertex *VertexRegistry::getVertex(const std::string &id) const {
    auto cluster = getClusterForId(id);
    if(!cluster){
        return nullptr;
    }
    return cluster->getVertex(id);
}

const Vertex *VertexRegistry::addVertex(const std::string &id) const {
    auto cluster = getClusterForId(id);
    if(!cluster){
        return nullptr;
    }
    return cluster->addVertex(id);
}

bool VertexRegistry::deleteVertex(const std::string &id) const {
    auto cluster = getClusterForId(id);
    if(!cluster){
        return false;
    }
    const Vertex *vertex = getVertex(id);
    if(!vertex){
        return false;
    }
    for(auto c : std::set<std::pair<std::string, std::string>>(vertex->getOutputConnections())){
        disconnectVertices(id, c.first, c.second);
    }
    for(auto c : std::set<std::pair<std::string, std::string>>(vertex->getInputConnections())){
        disconnectVertices(c.second, c.first, id);
    }
    return cluster->deleteVertex(id);
}

bool VertexRegistry::connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const {
    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);
    if(!cluster1 || !cluster2){
        return false;
    }
    const Vertex *vertex1 = getVertex(id1);
    const Vertex *vertex2 = getVertex(id2);
    if(!vertex1 || !vertex2){
        return false;
    }
    bool res = true;
    res &= cluster1->addConnection(vertex1->getId(), connName, vertex2->getId());
    res &= cluster2->addConnection(vertex2->getId(), connName, vertex1->getId(), true);
    return res;
}

bool VertexRegistry::disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const {
    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);
    if(!cluster1 || !cluster2){
        return false;
    }
    const Vertex *vertex1 = getVertex(id1);
    const Vertex *vertex2 = getVertex(id2);
    if(!vertex1 || !vertex2){
        return false;
    }
    bool res = true;
    res &= cluster1->removeConnection(vertex1->getId(), connName, vertex2->getId());
    res &= cluster2->removeConnection(vertex2->getId(), connName, vertex1->getId(), true);
    return res;
}

std::vector<std::string> VertexRegistry::getAllIds() const {
    std::vector<std::string> result;
    for (auto c : m_clusters){
        auto ids = c->getAllIds();
        result.insert(result.end(), ids.begin(), ids.end());
    }
    return result;
}

bool VertexRegistry::loadConfig() {
    fs::path config_dir("config");
    if(!fs::exists(config_dir)){
        return false;
    }
    fs::path filepath = config_dir / ("clusters.txt");
    if(!fs::exists(filepath)){
        return false;
    }
    std::ifstream inFile(filepath);
    inFile >> m_numClusters;
    return true;
}

VertexCluster *VertexRegistry::getClusterForId(const std::string &id) const {
    const size_t hash = m_hasher(id);
    for(auto c : m_clusters){
        auto range = c->getHashRange();
        if(hash >= range.first && hash <= range.second){
            return c;
        }
    }
    return nullptr;
}


