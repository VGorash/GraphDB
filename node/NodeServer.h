//
// Created by goras on 03.12.2023.
//

#ifndef DB_NODESERVER_H
#define DB_NODESERVER_H

#include "../server/AbstractServer.h"

class NodeServer : public AbstractServer {
public:
    NodeServer(const std::string &host, int port);

    ~NodeServer() override = default;

};


#endif //DB_NODESERVER_H
