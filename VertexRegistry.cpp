//
// Created by goras on 10.11.2023.
//

#include "VertexExceptions.h"
#include "VertexRegistry.h"
#include "VertexCluster.h"
#include "Vertex.h"

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
        throw std::exception("Cluster configuration error");
    }
    return cluster->getVertex(id);
}

const Vertex *VertexRegistry::addVertex(const std::string &id) const {
    auto cluster = getClusterForId(id);
    if(!cluster){
        throw std::exception("Cluster configuration error");
    }
    return cluster->addVertex(id);
}

void VertexRegistry::deleteVertex(const std::string &id) const {
    auto cluster = getClusterForId(id);
    if(!cluster){
        throw std::exception("Cluster configuration error");
    }
    const Vertex *vertex = getVertex(id);
    for(const auto& c : std::set<std::pair<std::string, std::string>>(vertex->getOutputConnections())){
        disconnectVertices(id, c.first, c.second);
    }
    for(const auto& c : std::set<std::pair<std::string, std::string>>(vertex->getInputConnections())){
        disconnectVertices(c.second, c.first, id);
    }
    cluster->deleteVertex(id);
}

void VertexRegistry::connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const {
    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);
    if(!cluster1 || !cluster2){
        throw std::exception("Cluster configuration error");
    }
    const Vertex *vertex1 = getVertex(id1);
    const Vertex *vertex2 = getVertex(id2);

    cluster1->addConnection(vertex1->getId(), connName, vertex2->getId());
    cluster2->addConnection(vertex2->getId(), connName, vertex1->getId(), true);
}

void VertexRegistry::disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const {
    auto cluster1 = getClusterForId(id1);
    auto cluster2 = getClusterForId(id2);
    if(!cluster1 || !cluster2){
        throw std::exception("Cluster configuration error");
    }
    const Vertex *vertex1 = getVertex(id1);
    const Vertex *vertex2 = getVertex(id2);

    cluster1->removeConnection(vertex1->getId(), connName, vertex2->getId());
    cluster2->removeConnection(vertex2->getId(), connName, vertex1->getId(), true);
}

std::vector<std::string> VertexRegistry::getAllIds() const {
    std::vector<std::string> result;
    for (auto c : m_clusters){
        auto ids = c->getAllIds();
        result.insert(result.end(), ids.begin(), ids.end());
    }
    return result;
}

void VertexRegistry::loadConfig() {
    fs::path config_dir("config");
    if(!fs::exists(config_dir)){
        throw std::exception("Cannot find config dir");
    }
    fs::path filepath = config_dir / ("clusters.txt");
    if(!fs::exists(filepath)){
        throw std::exception("Cannot find config file");
    }
    std::ifstream inFile(filepath);
    inFile >> m_numClusters;
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


