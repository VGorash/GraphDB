//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEXCLUSTER_H
#define DB_VERTEXCLUSTER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <future>
#include <mutex>

class Vertex;

class VertexCluster {

    friend class ClusterLocker;

public:

    explicit VertexCluster(std::pair<size_t, size_t> hashRange);

    VertexCluster(const VertexCluster &) = delete;

    void operator=(const VertexCluster &) = delete;

    ~VertexCluster();

    Vertex *getVertex(const std::string &id);

    Vertex *addVertex(const std::string &id);

    void deleteVertex(const std::string &id);

    void
    addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse = false);

    void
    removeConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse = false);

    std::vector<std::string> getAllIds();

    std::pair<size_t, size_t> getHashRange();

    Vertex createBackup(const std::string &id);

    void restoreBackup(const Vertex &vertex);

    void lock();

    void unlock();

private:

    void dump();

    void load();

private:
    std::hash<std::string> m_hasher;
    std::unordered_map<std::string, Vertex *> m_vertices;

    std::pair<size_t, size_t> m_hashRange;

    std::thread m_timer;
    std::mutex m_mutex;
    int m_usages;
    bool m_terminating;
    bool m_dirty;
};


#endif //DB_VERTEXCLUSTER_H
