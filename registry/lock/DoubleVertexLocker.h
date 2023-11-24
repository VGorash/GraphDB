//
// Created by goras on 25.11.2023.
//

#ifndef DB_DOUBLEVERTEXLOCKER_H
#define DB_DOUBLEVERTEXLOCKER_H

#include <string>
#include <mutex>
#include <unordered_set>

class DoubleVertexLocker {
public:
    DoubleVertexLocker(const std::string &id1, const std::string &id2, std::mutex &mutex,
                       std::unordered_set<std::string> &lockedVertices);

    ~DoubleVertexLocker();

private:
    const std::string &m_id1;
    const std::string &m_id2;
    std::mutex &m_mutex;
    std::unordered_set<std::string> &m_lockedVertices;
};


#endif //DB_DOUBLEVERTEXLOCKER_H
