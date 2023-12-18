//
// Created by goras on 24.11.2023.
//

#ifndef DB_VERTEXSTORAGE_H
#define DB_VERTEXSTORAGE_H

#include <string>
#include <vector>

class Vertex;

class VertexStorage {
public:
    virtual Vertex getVertex(const std::string &id) = 0;

    virtual Vertex addVertex(const std::string &id) = 0;

    virtual void deleteVertex(const std::string &id) = 0;

    virtual void
    addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) = 0;

    virtual void
    removeConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) = 0;

    virtual std::vector<std::string> getAllIds() = 0;

    virtual std::pair<size_t, size_t> getHashRange() const = 0;

    virtual void restoreBackup(const Vertex &vertex) = 0;

    virtual ~VertexStorage() = default;
};

#endif //DB_VERTEXSTORAGE_H
