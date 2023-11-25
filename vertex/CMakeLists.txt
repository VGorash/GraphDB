cmake_minimum_required(VERSION 3.25)

add_library(vertex STATIC
        Vertex.cpp
)
target_link_libraries(vertex PRIVATE utils)