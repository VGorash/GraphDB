//
// Created by goras on 10.11.2023.
//

#include "VertexRegistry.h"
#include "VertexCluster.h"
#include "Vertex.h"

#include <string>

VertexRegistry::VertexRegistry() {
    m_cluster = new VertexCluster({0, (size_t)-1});
}

VertexRegistry::~VertexRegistry() {
    m_cluster->dump();
    delete m_cluster;
}

const Vertex *VertexRegistry::getVertex(const std::string &id) const {
    return m_cluster->getVertex(id);
}

const Vertex *VertexRegistry::addVertex(const std::string &id) const {
    return m_cluster->addVertex(id);
}

bool VertexRegistry::deleteVertex(const std::string &id) const {
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
    return m_cluster->deleteVertex(id);
}

VertexRegistry &VertexRegistry::getInstance() {
    static VertexRegistry instance;
    return instance;
}

bool VertexRegistry::connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const {
    const Vertex *vertex1 = getVertex(id1);
    const Vertex *vertex2 = getVertex(id2);
    if(!vertex1 || !vertex2){
        return false;
    }
    bool res = true;
    res &= m_cluster->addConnection(vertex1->getId(), connName, vertex2->getId());
    res &= m_cluster->addConnection(vertex2->getId(), connName, vertex1->getId(), true);
    return res;
}

bool VertexRegistry::disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const {
    const Vertex *vertex1 = getVertex(id1);
    const Vertex *vertex2 = getVertex(id2);
    if(!vertex1 || !vertex2){
        return false;
    }
    bool res = true;
    res &= m_cluster->removeConnection(vertex1->getId(), connName, vertex2->getId());
    res &= m_cluster->removeConnection(vertex2->getId(), connName, vertex1->getId(), true);
    return res;
}

std::vector<std::string> VertexRegistry::getAllIds() const {
    return m_cluster->getAllIds();
}
