//
// Created by goras on 24.11.2023.
//

#include "Server.h"
#include <exception>

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

Server::Server(const std::string &host, int port) {

    WSADATA wsData;
    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (erStat != 0) {
        throw std::exception("Error WinSock version initializaion ");
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        closesocket(m_socket);
        WSACleanup();
        throw std::exception("Error on socket creation");
    }

    in_addr ip_to_num{};
    erStat = inet_pton(AF_INET, host.c_str(), &ip_to_num);
    if (erStat <= 0) {
        throw std::exception("Error in IP translation to special numeric format");
    }

    sockaddr_in servInfo{};
    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(port);

    erStat = bind(m_socket, (sockaddr *) &servInfo, sizeof(servInfo));
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

    SOCKET ClientConn = accept(m_socket, (sockaddr *) &clientInfo, &clientInfo_size);
    if (ClientConn == INVALID_SOCKET) {
        throw std::exception("Client detected, but can't connect to a client");
    }

    std::vector<char> servBuff(1024), clientBuff(1024);
    short packet_size = 0;

    while (true) {
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
        memcpy(clientBuff.data(), servBuff.data(), servBuff.size());

        // Check whether server would like to stop chatting
        if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
            shutdown(ClientConn, SD_BOTH);
            return;
        }

        packet_size = send(ClientConn, clientBuff.data(), clientBuff.size(), 0);

        if (packet_size == SOCKET_ERROR) {
            return;
        }

    }

}
