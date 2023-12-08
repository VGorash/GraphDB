//
// Created by goras on 20.11.2023.
//

#ifndef DB_VERTEXEXCEPTIONS_H
#define DB_VERTEXEXCEPTIONS_H

#include <stdexcept>
#include <utility>

enum VertexErrorCode {
    VertexAlreadyExists,
    VertexNotFound,
    VertexHasConnections,
    UnreachableStorage
};

enum ConnectionErrorCode {
    ConnectionAlreadyExists,
    ConnectionNotFound
};

class VertexOperationException : public std::exception {
public:
    explicit VertexOperationException(std::string vertexId, VertexErrorCode code)
            : std::exception(), vertexId(std::move(vertexId)), code(code) {};
    std::string vertexId;
    VertexErrorCode code;
};

class ConnectionOperationException : public std::exception {
public:
    ConnectionOperationException(std::string vertexId1,
                                 std::string vertexId2,
                                 std::string connName,
                                 bool reverse,
                                 ConnectionErrorCode code) : std::exception(),
                                                 vertexId1(std::move(vertexId1)),
                                                 vertexId2(std::move(vertexId2)),
                                                 connName(std::move(connName)),
                                                             reverse(reverse), code(code) {};

    ConnectionErrorCode code;
    std::string vertexId1;
    std::string vertexId2;
    std::string connName;
    bool reverse;
};

#endif //DB_VERTEXEXCEPTIONS_H
