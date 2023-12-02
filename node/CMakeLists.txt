cmake_minimum_required(VERSION 3.25)

add_executable(node
        run.cpp
        NodeServer.cpp
)

target_link_libraries(node PRIVATE server storage vertex utils)