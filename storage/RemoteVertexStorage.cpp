//
// Created by goras on 06.12.2023.
//

#include "RemoteVertexStorage.h"
#include "../vertex/VertexExceptions.h"
#include "../vertex/Vertex.h"
#include "../utils/string_utils.h"
#include "../node/codes.h"

#pragma comment(lib, "Ws2_32.lib")

void checkReply(std::vector<std::string> &parsed) {
    if (parsed[0] == std::to_string(Status::ConnectionError)) {
        throw ConnectionOperationException(parsed[2], parsed[4], parsed[3], parsed[5] == "0",
                                           static_cast<ConnectionErrorCode>(std::stoi(parsed[1])));
    }
    if (parsed[0] == std::to_string(Status::VertexError)) {
        throw VertexOperationException(parsed[2], static_cast<VertexErrorCode>(std::stoi(parsed[1])));
    }
}

RemoteVertexStorage::RemoteVertexStorage(const std::string &hostname, int port, int numConnections) {
    m_sockets.reserve(numConnections);
    m_servInfo = prepareServerInfo(hostname, port);
    try {
        for (int i = 0; i < numConnections; i++) {
            SOCKET socket = createSocket();
            connect(socket, m_servInfo);
            m_sockets.insert(socket);
        }
    }
    catch (std::exception &e) {
        throw RemoteStorageError("Cannot connect to server", this);
    }
    auto reply = processRequest("getHashRange", "");
    m_hashRange = {std::stoull(reply[1]), std::stoull(reply[2])};
}

RemoteVertexStorage::~RemoteVertexStorage() {
    for (auto s: m_sockets) {
        shutdown(s, SD_BOTH);
        closesocket(s);
    }
    WSACleanup();
}

std::string RemoteVertexStorage::processRequestImpl(ConnectionLock &lock, const std::string &request, bool retry) {
    try {
        const SOCKET &s = lock.getSocket();
        sendString(s, request);
        return receiveString(s);
    }
    catch (std::exception &e) {
        if (retry) {
            throw RemoteStorageError("Cannot connect to server", this);
        }
        try {
            shutdown(lock.getSocket(), SD_BOTH);
            closesocket(lock.getSocket());
            SOCKET s = createSocket();
            connect(s, m_servInfo);
            lock.setSocket(s);
        }
        catch (std::exception &nested) {
            throw RemoteStorageError("Cannot connect to server", this);
        }
        return processRequestImpl(lock, request, true);
    }
}

std::vector<std::string> RemoteVertexStorage::processRequest(const std::string &command, const std::string &data) {
    auto lock = ConnectionLock(this);
    auto reply = split(processRequestImpl(lock, command + " " + data), ' ');
    checkReply(reply);
    return reply;
}

Vertex RemoteVertexStorage::getVertex(const std::string &id) {
    auto reply = processRequest("getVertex", id);
    auto result = Vertex::fromString(reply[1]);
    return result;
}

Vertex RemoteVertexStorage::addVertex(const std::string &id) {
    auto reply = processRequest("addVertex", id);
    auto result = Vertex::fromString(reply[1]);
    return result;
}

void RemoteVertexStorage::deleteVertex(const std::string &id) {
    processRequest("deleteVertex", id);
}

void RemoteVertexStorage::addConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                        bool reverse) {
    processRequest("addConnection", id1 + " " + connName + " " + id2 + " " + (reverse ? "1" : "0"));
}

void RemoteVertexStorage::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                           bool reverse) {
    processRequest("removeConnection", id1 + " " + connName + " " + id2 + " " + +(reverse ? "1" : "0"));
}

std::vector<std::string> RemoteVertexStorage::getAllIds() {
    auto reply = processRequest("getAllIds", "");
    std::vector<std::string> result;
    result.reserve(reply.size() - 1);
    for (int i = 1; i < reply.size(); i++) {
        result.push_back(reply[i]);
    }
    return result;
}

std::pair<size_t, size_t> RemoteVertexStorage::getHashRange() const {
    return m_hashRange;
}

void RemoteVertexStorage::restoreBackup(const Vertex &vertex) {
    processRequest("restoreBackup", vertex.toString());
}

RemoteVertexStorage::ConnectionLock::ConnectionLock(RemoteVertexStorage *instance) {
    m_instance = instance;
    while (true) {
        if (instance->m_sockets.empty()) {
            continue;
        }
        instance->m_mutex.lock();
        if (instance->m_sockets.empty()) {
            instance->m_mutex.unlock();
            continue;
        }
        break;
    }
    m_socket = *instance->m_sockets.begin();
    instance->m_sockets.erase(m_socket);
    instance->m_mutex.unlock();
}

RemoteVertexStorage::ConnectionLock::~ConnectionLock() {
    m_instance->m_mutex.lock();
    m_instance->m_sockets.insert(m_socket);
    m_instance->m_mutex.unlock();
}

const SOCKET &RemoteVertexStorage::ConnectionLock::getSocket() const {
    return m_socket;
}

void RemoteVertexStorage::ConnectionLock::setSocket(SOCKET socket) {
    m_socket = socket;
}
