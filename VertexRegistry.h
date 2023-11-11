//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEXREGISTRY_H
#define DB_VERTEXREGISTRY_H

#include <string>
#include <vector>

class VertexCluster;
class Vertex;

class VertexRegistry {

public:
    static VertexRegistry &getInstance();

    const Vertex* getVertex(const std::string& id) const;
    const Vertex* addVertex(const std::string& id) const;
    bool deleteVertex(const std::string& id) const;

    bool connectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const;
    bool disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2) const;

    std::vector<std::string> getAllIds() const;

    VertexRegistry(const VertexRegistry&) = delete;
    void operator=(const VertexRegistry&) = delete;

private:
    VertexRegistry();
    ~VertexRegistry();

private:
    VertexCluster* m_cluster;
};


#endif //DB_VERTEXREGISTRY_H
