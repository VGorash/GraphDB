//
// Created by goras on 20.11.2023.
//

#include "VertexLocker.h"

VertexLocker::VertexLocker(const std::string &id, std::mutex &mutex,
                           std::unordered_set<std::string> &lockedVertices) : m_id(id), m_mutex(mutex),
                                                                              m_lockedVertices(lockedVertices) {
    while (true) {
        m_mutex.lock();
        if (m_lockedVertices.find(id) != m_lockedVertices.end()) {
            m_mutex.unlock();
            continue;
        }
        m_lockedVertices.insert(id);
        m_mutex.unlock();
        break;
    }
}

VertexLocker::~VertexLocker() {
    m_mutex.lock();
    m_lockedVertices.erase(m_id);
    m_mutex.unlock();
}
