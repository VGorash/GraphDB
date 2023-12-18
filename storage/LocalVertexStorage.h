//
// Created by goras on 24.11.2023.
//

#ifndef DB_LOCALVERTEXSTORAGE_H
#define DB_LOCALVERTEXSTORAGE_H


#include "VertexStorage.h"
#include "VertexStorageImpl.h"

class LocalVertexStorage : public VertexStorage {
public:
    explicit LocalVertexStorage(std::pair<size_t, size_t> hashRange);

    ~LocalVertexStorage() override;

    Vertex getVertex(const std::string &id) override;

    Vertex addVertex(const std::string &id) override;

    void deleteVertex(const std::string &id) override;

    void
    addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) override;

    void removeConnection(const std::string &id1, const std::string &connName, const std::string &id2,
                          bool reverse) override;

    std::vector<std::string> getAllIds() override;

    std::pair<size_t, size_t> getHashRange() const override;

    void restoreBackup(const Vertex &vertex) override;

private:
    VertexStorageImpl *m_impl;
};


#endif //DB_LOCALVERTEXSTORAGE_H
