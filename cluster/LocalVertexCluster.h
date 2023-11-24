//
// Created by goras on 24.11.2023.
//

#ifndef DB_LOCALVERTEXCLUSTER_H
#define DB_LOCALVERTEXCLUSTER_H


#include "VertexCluster.h"
#include "VertexClusterImpl.h"

class LocalVertexCluster : public VertexCluster {
public:
    explicit LocalVertexCluster(std::pair<size_t, size_t> hashRange);

    ~LocalVertexCluster() override;

    Vertex getVertex(const std::string &id) override;

    Vertex addVertex(const std::string &id) override;

    void deleteVertex(const std::string &id) override;

    void
    addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) override;

    void removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                          bool reverse) override;

    std::vector<std::string> getAllIds() override;

    std::pair<size_t, size_t> getHashRange() override;

    Vertex createBackup(const std::string &id) override;

    void restoreBackup(const Vertex &vertex) override;

    void lock() override;

    void unlock() override;

private:
    VertexClusterImpl *m_impl;
};


#endif //DB_LOCALVERTEXCLUSTER_H
