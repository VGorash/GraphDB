//
// Created by goras on 04.12.2023.
//

#ifndef DB_DATABASESERVER_H
#define DB_DATABASESERVER_H

#include "../server/AbstractServer.h"


class DatabaseServer : public AbstractServer {
public:
    DatabaseServer(const std::string &host, int port);
};


#endif //DB_DATABASESERVER_H
