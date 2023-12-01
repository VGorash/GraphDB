//
// Created by goras on 24.11.2023.
//

#include "Server.h"

#include "../vertex/Vertex.h"
#include "../utils/tcp_utils.h"

#include <exception>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

Server::Server(const std::string &host, int port) {

    m_socket = createSocket();
    sockaddr_in servInfo = prepareServerInfo(host, port);

    int erStat = bind(m_socket, (sockaddr *) &servInfo, sizeof(servInfo));
    if (erStat != 0) {
        closesocket(m_socket);
        WSACleanup();
        throw std::exception("Error Socket binding to server info");
    }

}

Server::~Server() {
    closesocket(m_socket);
    WSACleanup();
}

void Server::start() {
    int erStat = listen(m_socket, SOMAXCONN);
    if (erStat != 0) {
        throw std::exception("Can't start to start to");
    }

    sockaddr_in clientInfo{};
    ZeroMemory(&clientInfo, sizeof(clientInfo));
    int clientInfo_size = sizeof(clientInfo);

    while (true) {
        SOCKET ClientConn = accept(m_socket, (sockaddr *) &clientInfo, &clientInfo_size);
        if (ClientConn == INVALID_SOCKET) {
            throw std::exception("Client detected, but can't connect to a client");
        }

        std::thread([=]() {
            while (true) {
                try {
                    std::string s = receiveString(ClientConn);
                    if (s == "xxx") {
                        shutdown(ClientConn, SD_BOTH);
                        return;
                    }
                    sendString(ClientConn, s);
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                    shutdown(ClientConn, SD_BOTH);
                    return;
                }
            }
        }).detach();
    }
}
