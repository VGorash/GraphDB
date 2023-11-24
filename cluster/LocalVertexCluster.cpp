//
// Created by goras on 24.11.2023.
//

#include "LocalVertexCluster.h"
#include "../Vertex.h"

LocalVertexCluster::LocalVertexCluster(std::pair<size_t, size_t> hashRange) {
    m_impl = new VertexClusterImpl(hashRange);
}

LocalVertexCluster::~LocalVertexCluster() {
    delete m_impl;
}

Vertex LocalVertexCluster::getVertex(const std::string &id) {
    return *m_impl->getVertex(id);
}

Vertex LocalVertexCluster::addVertex(const std::string &id) {
    return *m_impl->addVertex(id);
}

void LocalVertexCluster::deleteVertex(const std::string &id) {
    m_impl->deleteVertex(id);
}

void LocalVertexCluster::addConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                       bool reverse) {
    m_impl->addConnection(id1, connName, id2, reverse);
}

void LocalVertexCluster::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                          bool reverse) {
    m_impl->removeConnection(id1, connName, id2, reverse);
}

std::vector<std::string> LocalVertexCluster::getAllIds() {
    return m_impl->getAllIds();
}

std::pair<size_t, size_t> LocalVertexCluster::getHashRange() {
    return m_impl->getHashRange();
}

Vertex LocalVertexCluster::createBackup(const std::string &id) {
    return getVertex(id);
}

void LocalVertexCluster::restoreBackup(const Vertex &vertex) {
    m_impl->restoreBackup(vertex);
}

void LocalVertexCluster::lock() {
    m_impl->lock();
}

void LocalVertexCluster::unlock() {
    m_impl->unlock();
}


