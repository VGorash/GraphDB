cmake_minimum_required(VERSION 3.25)
project(DB)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(registry SHARED
        registry/Vertex.cpp
        registry/cluster/VertexClusterImpl.cpp
        registry/VertexRegistry.cpp
        utils/string_utils.cpp
        registry/lock/VertexLocker.cpp
        registry/cluster/LocalVertexCluster.cpp)

add_executable(DB
        database/main.cpp
        registry/Vertex.cpp
)
target_link_libraries(DB PRIVATE registry)
