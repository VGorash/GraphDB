//
// Created by goras on 06.12.2023.
//

#ifndef DB_REMOTEVERTEXSTORAGE_H
#define DB_REMOTEVERTEXSTORAGE_H

#include "LocalVertexStorage.h"
#include "../utils/tcp_utils.h"
#include <mutex>
#include <unordered_set>

class RemoteStorageError : public std::exception {
public:
    explicit RemoteStorageError(const char *m) : std::exception(m) {};
};

class RemoteVertexStorage : public VertexStorage {

    class ConnectionLock {
    public:
        explicit ConnectionLock(RemoteVertexStorage *instance);

        ~ConnectionLock();

        const SOCKET &getSocket() const;

        void setSocket(SOCKET socket);

    private:
        SOCKET m_socket;

    private:
        RemoteVertexStorage *m_instance;
    };

public:
    RemoteVertexStorage(const std::string &hostname, int port, int numConnections);

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
    std::vector<std::string> processRequest(const std::string &command, const std::string &data);

    std::string processRequestImpl(ConnectionLock &lock, const std::string &request, bool retry = false);

private:
    sockaddr_in m_servInfo;
    std::unordered_set<SOCKET> m_sockets;
    std::mutex m_mutex;
    std::pair<size_t, size_t> m_hashRange;
};


#endif //DB_REMOTEVERTEXSTORAGE_H
