cmake_minimum_required(VERSION 3.25)

add_executable(node
        main.cpp
        Server.cpp
)

target_link_libraries(node PRIVATE storage vertex utils)