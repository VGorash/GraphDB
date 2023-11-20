//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEXREGISTRY_H
#define DB_VERTEXREGISTRY_H

#include <string>
#include <vector>
#include <unordered_map>

class VertexCluster;
class Vertex;

class VertexRegistry {

public:
    static VertexRegistry &getInstance();

    const Vertex* getVertex(const std::string& id) const;
    const Vertex* addVertex(const std::string& id) const;
    void deleteVertex(const std::string& id) const;

    void connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const;
    void disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const;

    std::vector<std::string> getAllIds() const;

    VertexRegistry(const VertexRegistry&) = delete;
    void operator=(const VertexRegistry&) = delete;

private:
    VertexRegistry();
    ~VertexRegistry();

    void loadConfig();

    VertexCluster *getClusterForId(const std::string& id) const;

private:
    int m_numClusters{};
    std::vector<VertexCluster*> m_clusters;
    std::hash<std::string> m_hasher;
};


#endif //DB_VERTEXREGISTRY_H
