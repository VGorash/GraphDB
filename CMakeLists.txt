cmake_minimum_required(VERSION 3.25)
project(DB)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(cluster STATIC
        registry/Vertex.cpp
        cluster/VertexClusterImpl.cpp
        cluster/LocalVertexCluster.cpp
)

add_library(registry STATIC
        registry/Vertex.cpp
        registry/VertexRegistry.cpp

        registry/lock/VertexLocker.cpp

        utils/string_utils.cpp
)

target_link_libraries(registry PRIVATE cluster)

add_executable(DB
        database/main.cpp
)

add_executable(test main.cpp)

add_executable(node
        node/main.cpp
        node/Server.cpp
)

target_link_libraries(DB PRIVATE registry)
