//
// Created by goras on 05.12.2023.
//

#include <exception>
#include <iostream>
#include <sstream>
#include <WinSock2.h>
#include "../utils/tcp_utils.h"

#pragma comment(lib, "Ws2_32.lib")

void connect(SOCKET ClientSock, sockaddr_in servInfo) {
    int erStat = connect(ClientSock, (sockaddr *) &servInfo, sizeof(servInfo));

    if (erStat != 0) {
        closesocket(ClientSock);
        throw std::exception("Cannot connect to server");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Hostname and port args are needed to start";
        exit(1);
    }
    int port;
    std::istringstream ss(argv[2]);
    if (!(ss >> port)) {
        std::cerr << "Invalid number: " << argv[2] << '\n';
        exit(1);
    } else if (!ss.eof()) {
        std::cerr << "Trailing characters after number: " << argv[2] << '\n';
        exit(1);
    }

    SOCKET ClientSock = createSocket();
    sockaddr_in servInfo = prepareServerInfo(std::string(argv[1]), port);
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
