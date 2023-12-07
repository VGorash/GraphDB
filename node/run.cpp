//
// Created by goras on 24.11.2023.
//

#include <iostream>
#include "NodeServer.h"
#include "../utils/string_utils.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Hostname, port and hash range args are needed to start";
        exit(1);
    }
    auto port = convertToNumber<int>(argv[2]);
    auto hashMin = convertToNumber<size_t>(argv[3]);
    auto hashMax = convertToNumber<size_t>(argv[4]);

    auto server = NodeServer(argv[1], port, hashMin, hashMax);
    server.start();
}
