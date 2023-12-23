//
// Created by goras on 15.12.2023.
//
#include <chrono>
#include <iostream>
#include <sstream>
#include <WinSock2.h>
#include <thread>
#include <vector>
#include "../utils/tcp_utils.h"

#pragma comment(lib, "Ws2_32.lib")

#define ITEMS_PER_THREAD 10000
#define CONNECTIONS_PER_THREAD 10000

void testcase(int num_threads, sockaddr_in servInfo) {
    std::chrono::steady_clock::time_point begin_global;
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    std::cout << "Creating vertices: " << num_threads * ITEMS_PER_THREAD << " values in " << num_threads << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    begin_global = std::chrono::steady_clock::now();

    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock, "add " + std::to_string(i));
                auto answer = receiveString(ClientSock);
                if (answer != "Vertex added\n") {
                    std::cout << answer << std::endl;
                }
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
//    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
//              << " milliseconds" << std::endl;
//    std::cout << std::endl;

    threads.clear();
    threads.reserve(num_threads);

    std::cout << "Reading vertices: " << num_threads * ITEMS_PER_THREAD << " values in " << num_threads << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock, "read " + std::to_string(i));
                auto answer = receiveString(ClientSock);
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
//    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
//              << " milliseconds" << std::endl;
//    std::cout << std::endl;

    threads.clear();
    threads.reserve(num_threads);

    std::cout << "Making connections: " << num_threads * ITEMS_PER_THREAD << " values in " << num_threads << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock,
                           "connect " + std::to_string(i) + " c " + std::to_string(num_threads * ITEMS_PER_THREAD - i));
                auto answer = receiveString(ClientSock);
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
//    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
//              << " milliseconds" << std::endl;
//    std::cout << std::endl;

    threads.clear();
    threads.reserve(num_threads);

    std::cout << "Making query to all connections: " << num_threads * ITEMS_PER_THREAD << " values in " << num_threads
              << " threads" << std::endl;
    begin = std::chrono::steady_clock::now();
    {
        SOCKET ClientSock = createSocket();
        connect(ClientSock, servInfo);
        sendString(ClientSock, "query ? ? ?");
        auto answer = receiveString(ClientSock);
        //std::cout << answer;
    }
    end = std::chrono::steady_clock::now();
//    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
//              << " milliseconds" << std::endl;
//    std::cout << std::endl;

    std::cout << "Deleting connections: " << num_threads * ITEMS_PER_THREAD << " values in " << num_threads
              << " threads" << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock, "disconnect " + std::to_string(i) + " c " +
                                       std::to_string(num_threads * ITEMS_PER_THREAD - i));
                auto answer = receiveString(ClientSock);
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
//    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
//              << " milliseconds" << std::endl;
//    std::cout << std::endl;

    threads.clear();
    threads.reserve(num_threads);

    std::cout << "Deleting vertices: " << num_threads * ITEMS_PER_THREAD << " values in " << num_threads << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock, "delete " + std::to_string(i));
                auto answer = receiveString(ClientSock);
                if (answer != "Vertex successfully deleted\n") {
                    std::cout << answer << std::endl;
                }
            }
        });
    }

    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
    //std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
    //<< " milliseconds" << std::endl;
    //std::cout << std::endl;

    std::cout << "Total time " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_global).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;
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
    sockaddr_in servInfo = prepareServerInfo(std::string(argv[1]), port);

    for (int i = 1; i < 11; i++) {
        testcase(i, servInfo);
    }

}