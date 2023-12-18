//
// Created by goras on 25.11.2023.
//

#include "DoubleVertexLocker.h"

DoubleVertexLocker::DoubleVertexLocker(const std::string &id1, const std::string &id2, std::mutex &mutex,
                                       std::unordered_set<std::string> &lockedVertices) : m_id1(id1), m_id2(id2),
                                                                                          m_mutex(mutex),
                                                                                          m_lockedVertices(
                                                                                                  lockedVertices) {
    while (true) {
        m_mutex.lock();
        if (m_lockedVertices.find(id1) != m_lockedVertices.end()) {
            m_mutex.unlock();
            continue;
        }
        if (m_lockedVertices.find(id2) != m_lockedVertices.end()) {
            m_mutex.unlock();
            continue;
        }
        m_lockedVertices.insert(id1);
        m_lockedVertices.insert(id2);
        m_mutex.unlock();
        break;
    }
}

DoubleVertexLocker::~DoubleVertexLocker() {
    m_mutex.lock();
    m_lockedVertices.erase(m_id1);
    if (m_id1 != m_id2) {
        m_lockedVertices.erase(m_id2);
    }
    m_mutex.unlock();
}
