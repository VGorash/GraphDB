//
// Created by goras on 24.11.2023.
//

#include "AbstractServer.h"

#include "../node/codes.h"
#include "../vertex/Vertex.h"
#include "../storage/VertexStorageImpl.h"
#include "../utils/tcp_utils.h"
#include "../utils/string_utils.h"

#include <exception>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

AbstractServer::AbstractServer(const std::string &host, int port) {

    m_socket = createSocket();
    sockaddr_in servInfo = prepareServerInfo(host, port);

    int erStat = bind(m_socket, (sockaddr *) &servInfo, sizeof(servInfo));
    if (erStat != 0) {
        closesocket(m_socket);
        WSACleanup();
        throw std::exception("Error Socket binding to server info");
    }

}

AbstractServer::~AbstractServer() {
    closesocket(m_socket);
    WSACleanup();
}

void AbstractServer::start() {
    int erStat = listen(m_socket, SOMAXCONN);
    if (erStat != 0) {
        throw std::exception("Can't start");
    }

    sockaddr_in clientInfo{};
    ZeroMemory(&clientInfo, sizeof(clientInfo));
    int clientInfo_size = sizeof(clientInfo);

    while (true) {
        SOCKET conn = accept(m_socket, (sockaddr *) &clientInfo, &clientInfo_size);
        if (conn == INVALID_SOCKET) {
            std::cout << "Client detected, but can't connect to a client\n";
            continue;
        }

        std::thread([=]() {
            while (true) {
                try {
                    std::string s = receiveString(conn);
                    std::string result = processString(s);
                    sendString(conn, result);
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                    shutdown(conn, SD_BOTH);
                    return;
                }
            }
        }).detach();
    }
}

std::string AbstractServer::processString(const std::string &s) {
    auto parsed = split(s);
    if (m_operations.find(parsed[0]) != m_operations.end()) {
        return m_operations[parsed[0]](parsed);
    }
    return m_fallbackOperation(parsed);
}
