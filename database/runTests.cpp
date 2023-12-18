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

#define NUM_THREADS 5
#define ITEMS_PER_THREAD 10000
#define CONNECTIONS_PER_THREAD 10000

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

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    sockaddr_in servInfo = prepareServerInfo(std::string(argv[1]), port);

    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

    std::cout << "Creating vertices: " << NUM_THREADS * ITEMS_PER_THREAD << " values in " << NUM_THREADS << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < NUM_THREADS; t++) {
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
    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;

    threads.clear();
    threads.reserve(NUM_THREADS);

    std::cout << "Reading vertices: " << NUM_THREADS * ITEMS_PER_THREAD << " values in " << NUM_THREADS << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < NUM_THREADS; t++) {
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
    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;

    threads.clear();
    threads.reserve(NUM_THREADS);

    std::cout << "Making connections: " << NUM_THREADS * ITEMS_PER_THREAD << " values in " << NUM_THREADS << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < NUM_THREADS; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock,
                           "connect " + std::to_string(i) + " c " + std::to_string(NUM_THREADS * ITEMS_PER_THREAD - i));
                auto answer = receiveString(ClientSock);
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;

    threads.clear();
    threads.reserve(NUM_THREADS);

    std::cout << "Making query to all connections: " << NUM_THREADS * ITEMS_PER_THREAD << " values in " << NUM_THREADS
              << " threads" << std::endl;
    begin = std::chrono::steady_clock::now();
    {
        SOCKET ClientSock = createSocket();
        connect(ClientSock, servInfo);
        sendString(ClientSock, "query ? ? ?");
        auto answer = receiveString(ClientSock);
        std::cout << answer;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;

    std::cout << "Deleting connections: " << NUM_THREADS * ITEMS_PER_THREAD << " values in " << NUM_THREADS
              << " threads" << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < NUM_THREADS; t++) {
        threads.emplace_back([=]() {
            SOCKET ClientSock = createSocket();
            connect(ClientSock, servInfo);

            for (int i = t * ITEMS_PER_THREAD; i < (t + 1) * ITEMS_PER_THREAD; i++) {
                sendString(ClientSock, "disconnect " + std::to_string(i) + " c " +
                                       std::to_string(NUM_THREADS * ITEMS_PER_THREAD - i));
                auto answer = receiveString(ClientSock);
            }
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;

    threads.clear();
    threads.reserve(NUM_THREADS);

    std::cout << "Deleting vertices: " << NUM_THREADS * ITEMS_PER_THREAD << " values in " << NUM_THREADS << " threads"
              << std::endl;
    begin = std::chrono::steady_clock::now();
    for (int t = 0; t < NUM_THREADS; t++) {
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
    std::cout << "Operation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << " milliseconds" << std::endl;
    std::cout << std::endl;
}