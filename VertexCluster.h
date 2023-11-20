//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEXCLUSTER_H
#define DB_VERTEXCLUSTER_H

#include <unordered_map>
#include <string>
#include <vector>

class Vertex;

class VertexCluster {

    friend class VertexRegistry;

public:
    VertexCluster(const VertexCluster &) = delete;

    void operator=(const VertexCluster &) = delete;

private:
    Vertex *getVertex(const std::string &id);

    Vertex *addVertex(const std::string &id);

    void deleteVertex(const std::string &id);

    void
    addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse = false);

    void
    removeConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse = false);

    std::vector<std::string> getAllIds();

    void dump() const;

    void load();

    std::pair<size_t, size_t> getHashRange();

    explicit VertexCluster(std::pair<size_t, size_t> hashRange);

    ~VertexCluster();

private:
    std::hash<std::string> m_hasher;
    std::unordered_map<std::string, Vertex *> m_vertices;

    std::pair<size_t, size_t> m_hashRange;
};


#endif //DB_VERTEXCLUSTER_H
