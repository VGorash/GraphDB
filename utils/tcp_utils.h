//
// Created by goras on 30.11.2023.
//

#ifndef DB_TCP_UTILS_H
#define DB_TCP_UTILS_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

SOCKET createSocket();

sockaddr_in prepareServerInfo(const std::string &hostname, int port);

void sendString(SOCKET socket, const std::string &string);

std::string receiveString(SOCKET socket);

#endif //DB_TCP_UTILS_H
