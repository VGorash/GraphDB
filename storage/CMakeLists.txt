cmake_minimum_required(VERSION 3.25)

add_library(storage STATIC
        VertexStorageImpl.cpp
        LocalVertexStorage.cpp
        RemoteVertexStorage.cpp
        StorageLocker.cpp
)
target_link_libraries(storage PRIVATE vertex utils)