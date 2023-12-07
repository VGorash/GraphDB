//
// Created by goras on 06.12.2023.
//

#ifndef DB_REMOTEVERTEXSTORAGE_H
#define DB_REMOTEVERTEXSTORAGE_H

#include "LocalVertexStorage.h"
#include "../utils/tcp_utils.h"

class RemoteVertexStorage : public VertexStorage {
public:
    RemoteVertexStorage(const std::string &hostname, int port);

    ~RemoteVertexStorage() override;

    Vertex getVertex(const std::string &id) override;

    Vertex addVertex(const std::string &id) override;

    void deleteVertex(const std::string &id) override;

    void
    addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) override;

    void
    removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                     bool reverse) override;

    std::vector<std::string> getAllIds() override;

    std::pair<size_t, size_t> getHashRange() override;

    Vertex createBackup(const std::string &id) override;

    void restoreBackup(const Vertex &vertex) override;

    void lock() override;

    void unlock() override;

private:
    SOCKET &getSocket();

private:
    SOCKET m_socket;
};


#endif //DB_REMOTEVERTEXSTORAGE_H
