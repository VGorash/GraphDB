cmake_minimum_required(VERSION 3.25)

add_executable(DBStandalone
        runStandalone.cpp
)

add_executable(DBServer
        DatabaseServer.cpp
        runServer.cpp
)

add_executable(DBClient
        runClient.cpp
)

target_link_libraries(DBStandalone PRIVATE registry utils)
target_link_libraries(DBServer PRIVATE registry utils server)
target_link_libraries(DBClient PRIVATE utils)