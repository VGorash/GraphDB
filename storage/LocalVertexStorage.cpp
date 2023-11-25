//
// Created by goras on 24.11.2023.
//

#include "LocalVertexStorage.h"
#include "../vertex/Vertex.h"

LocalVertexStorage::LocalVertexStorage(std::pair<size_t, size_t> hashRange) {
    m_impl = new VertexStorageImpl(hashRange);
}

LocalVertexStorage::~LocalVertexStorage() {
    delete m_impl;
}

Vertex LocalVertexStorage::getVertex(const std::string &id) {
    return *m_impl->getVertex(id);
}

Vertex LocalVertexStorage::addVertex(const std::string &id) {
    return *m_impl->addVertex(id);
}

void LocalVertexStorage::deleteVertex(const std::string &id) {
    m_impl->deleteVertex(id);
}

void LocalVertexStorage::addConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                       bool reverse) {
    m_impl->addConnection(id1, connName, id2, reverse);
}

void LocalVertexStorage::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                          bool reverse) {
    m_impl->removeConnection(id1, connName, id2, reverse);
}

std::vector<std::string> LocalVertexStorage::getAllIds() {
    return m_impl->getAllIds();
}

std::pair<size_t, size_t> LocalVertexStorage::getHashRange() {
    return m_impl->getHashRange();
}

Vertex LocalVertexStorage::createBackup(const std::string &id) {
    return getVertex(id);
}

void LocalVertexStorage::restoreBackup(const Vertex &vertex) {
    m_impl->restoreBackup(vertex);
}

void LocalVertexStorage::lock() {
    m_impl->lock();
}

void LocalVertexStorage::unlock() {
    m_impl->unlock();
}


