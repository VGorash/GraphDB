//
// Created by goras on 30.11.2023.
//

#include "tcp_utils.h"
#include <exception>
#include <vector>

void sendData(SOCKET s, const char *buf, int len) {
    int packet_size = send(s, buf, len, 0);
    if (packet_size == SOCKET_ERROR) {
        throw std::exception("Error on sending");
    }
}

void receiveData(SOCKET s, char *buf, int len) {
    int packet_size = recv(s, buf, len, 0);
    if (packet_size == SOCKET_ERROR) {
        throw std::exception("Error on receiving");
    }
}

SOCKET createSocket() {
    WSADATA wsData;
    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (erStat != 0) {
        throw std::exception("Error WinSock version initializaion ");
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        closesocket(s);
        WSACleanup();
        throw std::exception("Error on socket creation");
    }
    return s;
}

sockaddr_in prepareServerInfo(const std::string &hostname, int port) {
    in_addr ip_to_num{};
    int erStat = inet_pton(AF_INET, hostname.c_str(), &ip_to_num);
    if (erStat <= 0) {
        throw std::exception("Error in IP translation to special numeric format");
    }

    sockaddr_in servInfo{};
    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(port);

    return servInfo;
}

void sendString(SOCKET socket, const std::string &string) {
    int string_size = (int) string.size();
    sendData(socket, (const char *) &string_size, sizeof(int));
    sendData(socket, string.c_str(), string_size);
}

std::string receiveString(SOCKET socket) {
    int string_size;
    std::vector<char> buffer;
    receiveData(socket, (char *) &string_size, sizeof(int));

    if (string_size < 0) {
        throw std::exception("Invalid string size");
    }

    buffer.resize(string_size);
    receiveData(socket, buffer.data(), string_size);

    return {buffer.begin(), buffer.end()};
}

void connect(SOCKET ClientSock, sockaddr_in servInfo) {
    int erStat = connect(ClientSock, (sockaddr *) &servInfo, sizeof(servInfo));

    if (erStat != 0) {
        closesocket(ClientSock);
        throw std::exception("Cannot connect to server");
    }
}
