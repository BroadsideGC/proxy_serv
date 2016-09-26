cmake_minimum_required(VERSION 2.8)
project(proxy)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3 -D_GLIBCXX_DEBUG -fsanitize=address,undefined")

set(SOURCE_FILES main.cpp
        file_descriptor.cpp file_descriptor.h
        linux_socket.cpp linux_socket.h
        server_utils.h
        proxy_server.cpp proxy_server.h
        epoll_io.cpp epoll_io.h
        io_event.cpp io_event.h
        http.hpp http.cpp
        lru_cache.h
        resolver.cpp resolver.h
        client.h client.cpp
        server.h server.cpp
        timer.h)
add_executable(proxy ${SOURCE_FILES})
