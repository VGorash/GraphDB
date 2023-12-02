//
// Created by goras on 03.12.2023.
//

#include "NodeServer.h"
#include <string>
#include <vector>

std::string operation(const std::vector<std::string> &s) {
    std::string result;
    for (int i = 1; i < s.size(); i++) {
        result += s[i] + " ";
    }
    return result;
}

std::string fallback(const std::vector<std::string> &s) {
    return "Command not found";
}

NodeServer::NodeServer(const std::string &host, int port) : AbstractServer(host, port) {
    m_operations.insert({"a", operation});
    m_fallbackOperation = fallback;
}
