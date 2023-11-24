//
// Created by goras on 24.11.2023.
//

#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include <WinSock2.h>
#include <string>

class Server {
public:
    Server(const std::string &host, int port);

    ~Server();

    void start();

private:
    SOCKET m_socket;

};


#endif //TEST_SERVER_H
