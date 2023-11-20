//
// Created by goras on 20.11.2023.
//

#ifndef DB_VERTEXLOCKER_H
#define DB_VERTEXLOCKER_H

#include <string>
#include <mutex>
#include <unordered_set>


class VertexLocker {
public:
    VertexLocker(const std::string &id, std::mutex &mutex, std::unordered_set<std::string> &lockedVertices);

    ~VertexLocker();

private:
    const std::string &m_id;
    std::mutex &m_mutex;
    std::unordered_set<std::string> &m_lockedVertices;
};


#endif //DB_VERTEXLOCKER_H
