project(libsocket)
cmake_minimum_required(VERSION 3.2)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/bin)

include_directories(.)

add_library(socket SHARED socket.cpp)

add_executable(client test/client.cpp)
target_link_libraries(client PRIVATE socket)

add_executable(server test/server.cpp)
target_link_libraries(server PRIVATE socket)
