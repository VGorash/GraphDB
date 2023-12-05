//
// Created by goras on 03.12.2023.
//

#ifndef DB_NODESERVER_H
#define DB_NODESERVER_H

#include <sstream>

#include "../server/AbstractServer.h"
#include "../storage/VertexStorageImpl.h"

class NodeServer : public AbstractServer {
public:
    NodeServer(const std::string &host, int port);

    ~NodeServer() override;

private:
    std::string processString(const std::string &command) override;

private:
    VertexStorageImpl *m_storage;
    std::map<std::string, std::function<void(const std::vector<std::string> &,
                                             VertexStorageImpl *storage,
                                             std::stringstream &out)>> m_operations;

};


#endif //DB_NODESERVER_H
