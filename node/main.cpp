//
// Created by goras on 24.11.2023.
//

#include <iostream>
#include "Server.h"

int main() {
    auto server = Server("127.0.0.1", 26080);
    server.start();
}
