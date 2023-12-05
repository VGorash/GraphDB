//
// Created by goras on 24.11.2023.
//

#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include <WinSock2.h>
#include <string>
#include <map>
#include <functional>

class AbstractServer {
public:
    AbstractServer(const std::string &host, int port);

    virtual ~AbstractServer();

    void start();

protected:
    virtual std::string processString(const std::string &command) = 0;

private:
    SOCKET m_socket;
};


#endif //TEST_SERVER_H
