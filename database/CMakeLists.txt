cmake_minimum_required(VERSION 3.25)

add_executable(DB
        main.cpp
)

target_link_libraries(DB PRIVATE registry utils)