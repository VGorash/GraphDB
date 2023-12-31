//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEXREGISTRY_H
#define DB_VERTEXREGISTRY_H

#include <string>
#include <vector>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

class VertexStorage;

class Vertex;

class RemoteStorageError;

class VertexRegistry {

public:
    static VertexRegistry &getInstance();

    Vertex getVertex(const std::string &id);

    Vertex addVertex(const std::string &id);

    void deleteVertex(const std::string &id);

    void connectVertices(const std::string &id1, const std::string &connName, const std::string &id2);

    void disconnectVertices(const std::string &id1, const std::string &connName, const std::string &id2);

    std::vector<std::string> getAllIds();

    VertexRegistry(const VertexRegistry &) = delete;

    void operator=(const VertexRegistry &) = delete;

private:
    VertexRegistry();

    ~VertexRegistry();

    VertexStorage *getClusterForId(const std::string &id);

    Vertex getVertexNoLock(const std::string &id);

    void processRemoteError(RemoteStorageError &e);

    void transferVertices(VertexStorage *src, std::vector<VertexStorage *> dsts);

    void checkIntersections();

private:
    std::vector<VertexStorage *> m_storages;
    VertexStorage *m_fallbackStorage;
    std::hash<std::string> m_hasher;

    std::unordered_set<std::string> m_lockedVertices;
    std::mutex m_mutex;

    std::unordered_set<VertexStorage *> m_invalidStorages;
};


#endif //DB_VERTEXREGISTRY_H
