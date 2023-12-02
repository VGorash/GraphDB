//
// Created by goras on 24.11.2023.
//

#include <exception>

#include <iostream>
#include <WinSock2.h>
#include "utils/tcp_utils.h"

#pragma comment(lib, "Ws2_32.lib")

void connect(SOCKET ClientSock, sockaddr_in servInfo) {
    int erStat = connect(ClientSock, (sockaddr *) &servInfo, sizeof(servInfo));

    if (erStat != 0) {
        closesocket(ClientSock);
        throw std::exception("Cannot connect to server");
    }
}

int main() {
    SOCKET ClientSock = createSocket();
    sockaddr_in servInfo = prepareServerInfo("127.0.0.1", 26080);
    connect(ClientSock, servInfo);

    // The size of sending / receiving packet in bytes\

    std::string input;

    while (true) {

        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            shutdown(ClientSock, SD_BOTH);
            closesocket(ClientSock);
            WSACleanup();
            return 0;
        }

        sendString(ClientSock, input);
        auto reply = receiveString(ClientSock);

        std::cout << reply << std::endl;

    }
    closesocket(ClientSock);
    WSACleanup();

    return 0;
}
