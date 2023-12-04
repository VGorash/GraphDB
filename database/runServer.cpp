//
// Created by goras on 04.12.2023.
//

#include "DatabaseServer.h"
#include <iostream>
#include <sstream>

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

    auto server = DatabaseServer(argv[1], port);
    server.start();
}
