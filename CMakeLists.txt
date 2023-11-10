cmake_minimum_required(VERSION 3.25)
project(DB)

set(CMAKE_CXX_STANDARD 17)

add_executable(DB
        main.cpp
        Node.cpp
        NodeCluster.cpp
        NodeRegistry.cpp
        DB.cpp
        utils.cpp
)
