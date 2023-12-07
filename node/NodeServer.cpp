//
// Created by goras on 03.12.2023.
//

#include <string>
#include <vector>
#include "NodeServer.h"
#include "codes.h"
#include "../utils/string_utils.h"
#include "../vertex/Vertex.h"
#include "../vertex/VertexExceptions.h"

void addVertex(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    Vertex *vertex = storage->addVertex(s[1]);
    out << Status::Success << " " << vertex->toString();
}

void getVertex(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    Vertex *vertex = storage->getVertex(s[1]);
    out << Status::Success << " " << vertex->toString();
}

void deleteVertex(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    storage->deleteVertex(s[1]);
    out << Status::Success;
}

void addConnection(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    storage->addConnection(s[1], s[2], s[3], s[4] != "0");
    out << Status::Success;
}

void removeConnection(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    storage->removeConnection(s[1], s[2], s[3], s[4] != "0");
    out << Status::Success;
}

void getAllIds(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    auto ids = storage->getAllIds();
    out << Status::Success;
    for (const auto &i: ids) {
        out << " " << i;
    }
}

void getHashRange(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    auto range = storage->getHashRange();
    out << Status::Success << " " << range.first << " " << range.second;
}

void createBackup(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    Vertex vertex = storage->createBackup(s[1]);
    out << Status::Success << " " << vertex.toString();
}

void restoreBackup(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    Vertex vertex = Vertex::fromString(s[1]);
    storage->restoreBackup(vertex);
    out << Status::Success;
}

void lock(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    storage->lock();
    out << Status::Success;
}

void unlock(const std::vector<std::string> &s, VertexStorageImpl *storage, std::stringstream &out) {
    storage->unlock();
    out << Status::Success;
}

std::string fallback(const std::vector<std::string> &s) {
    return "Command not found";
}

NodeServer::NodeServer(const std::string &host, int port) : AbstractServer(host, port) {
    m_storage = new VertexStorageImpl({0, (size_t) -1});

    m_operations.insert({"addVertex", addVertex});
    m_operations.insert({"getVertex", getVertex});
    m_operations.insert({"deleteVertex", deleteVertex});
    m_operations.insert({"addConnection", addConnection});
    m_operations.insert({"removeConnection", removeConnection});
    m_operations.insert({"getAllIds", getAllIds});
    m_operations.insert({"getHashRange", getHashRange});
    m_operations.insert({"createBackup", createBackup});
    m_operations.insert({"restoreBackup", restoreBackup});
    m_operations.insert({"lock", lock});
    m_operations.insert({"unlock", unlock});
}

NodeServer::~NodeServer() {
    delete m_storage;
}

std::string NodeServer::processString(const std::string &command) {
    auto parsed = split(command);
    if (m_operations.find(parsed[0]) != m_operations.end()) {
        std::stringstream out;
        try {
            m_operations[parsed[0]](parsed, m_storage, out);
        }
        catch (VertexOperationException &e) {
            out << Status::VertexError << " " << e.code << " " << e.vertexId;
        }
        catch (ConnectionOperationException &e) {
            out << Status::ConnectionError << " " << e.code << " " << e.vertexId1 << " " << e.connName << " "
                << e.vertexId2 << e.reverse;
        }
        return out.str();
    }
    return fallback(parsed);
}
