//
// Created by goras on 20.11.2023.
//

#ifndef DB_VERTEXEXCEPTIONS_H
#define DB_VERTEXEXCEPTIONS_H

#include <stdexcept>
#include <utility>

class VertexOperationException : public std::exception {
public:
    explicit VertexOperationException(std::string vertexId) : std::exception(), vertexId(std::move(vertexId)) {};
    std::string vertexId;
};

class ConnectionOperationException : public std::exception {
public:
    ConnectionOperationException(std::string vertexId1,
                                 std::string vertexId2,
                                 std::string connName,
                                 bool reverse) : std::exception(),
                                                 vertexId1(std::move(vertexId1)),
                                                 vertexId2(std::move(vertexId2)),
                                                 connName(std::move(connName)),
                                                 reverse(reverse) {};

    std::string vertexId1;
    std::string vertexId2;
    std::string connName;
    bool reverse;
};

#endif //DB_VERTEXEXCEPTIONS_H
