cmake_minimum_required(VERSION 3.25)

add_library(registry STATIC
        VertexRegistry.cpp
        lock/VertexLocker.cpp
        lock/DoubleVertexLocker.cpp
)
target_link_libraries(registry PRIVATE storage vertex)