cmake_minimum_required(VERSION 3.25)
project(DB)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(utils)
add_subdirectory(vertex)
add_subdirectory(storage)
add_subdirectory(registry)
add_subdirectory(database)
add_subdirectory(server)
add_subdirectory(node)