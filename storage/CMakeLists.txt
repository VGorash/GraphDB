cmake_minimum_required(VERSION 3.25)

add_library(cluster STATIC
        VertexStorageImpl.cpp
        LocalVertexStorage.cpp
)
target_link_libraries(cluster PRIVATE vertex)