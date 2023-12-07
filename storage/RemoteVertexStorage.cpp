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

RemoteVertexStorage::RemoteVertexStorage(const std::string &hostname, int port) {
    m_socket = createSocket();
    sockaddr_in servInfo = prepareServerInfo(hostname, port);
    connect(m_socket, servInfo);
}

RemoteVertexStorage::~RemoteVertexStorage() {
    shutdown(m_socket, SD_BOTH);
    closesocket(m_socket);
    WSACleanup();
}

SOCKET &RemoteVertexStorage::getSocket() {
    return m_socket;
}

Vertex RemoteVertexStorage::getVertex(const std::string &id) {
    SOCKET &s = getSocket();
    sendString(s, std::string("getVertex ") + id);
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
    auto result = Vertex::fromString(reply[1]);
    return result;
}

Vertex RemoteVertexStorage::addVertex(const std::string &id) {
    SOCKET &s = getSocket();
    sendString(s, std::string("addVertex ") + id);
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
    auto result = Vertex::fromString(reply[1]);
    return result;
}

void RemoteVertexStorage::deleteVertex(const std::string &id) {
    SOCKET &s = getSocket();
    sendString(s, std::string("deleteVertex ") + id);
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
}

void RemoteVertexStorage::addConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                        bool reverse) {
    SOCKET &s = getSocket();
    sendString(s,
               std::string("addConnection ") + id1 + " " + connName + " " + id2 + " " + std::to_string((int) reverse));
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
}

void RemoteVertexStorage::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                                           bool reverse) {
    SOCKET &s = getSocket();
    sendString(s, std::string("removeConnection ") + id1 + " " + connName + " " + id2 + " " +
                  std::to_string((int) reverse));
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
}

std::vector<std::string> RemoteVertexStorage::getAllIds() {
    SOCKET &s = getSocket();
    sendString(s, "getAllIds");
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
    std::vector<std::string> result;
    result.reserve(reply.size() - 1);
    for (int i = 1; i < reply.size(); i++) {
        result.push_back(reply[i]);
    }
    return result;
}

std::pair<size_t, size_t> RemoteVertexStorage::getHashRange() {
    SOCKET &s = getSocket();
    sendString(s, "getHashRange");
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
    return {std::stoull(reply[1]), std::stoull(reply[2])};
}

Vertex RemoteVertexStorage::createBackup(const std::string &id) {
    SOCKET &s = getSocket();
    sendString(s, std::string("createBackup ") + id);
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
    auto result = Vertex::fromString(reply[1]);
    return result;
}

void RemoteVertexStorage::restoreBackup(const Vertex &vertex) {
    SOCKET &s = getSocket();
    sendString(s, std::string("restoreBackup ") + vertex.toString());
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
}

void RemoteVertexStorage::lock() {
    SOCKET &s = getSocket();
    sendString(s, std::string("lock"));
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
}

void RemoteVertexStorage::unlock() {
    SOCKET &s = getSocket();
    sendString(s, std::string("unlock"));
    auto reply = split(receiveString(s), ' ');
    checkReply(reply);
}
