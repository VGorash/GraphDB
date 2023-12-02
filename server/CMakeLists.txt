cmake_minimum_required(VERSION 3.25)

add_library(server STATIC AbstractServer.cpp)

target_link_libraries(server PRIVATE utils)