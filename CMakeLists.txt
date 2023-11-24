cmake_minimum_required(VERSION 3.25)
project(DB)

set(CMAKE_CXX_STANDARD 17)

add_executable(DB
        main.cpp
        Vertex.cpp
        cluster/VertexClusterImpl.cpp
        VertexRegistry.cpp
        utils.cpp
        lock/VertexLocker.cpp
        lock/ClusterLocker.h
)
