//
// Created by bigz on 03.08.16.
//

#ifndef PROXY_LINUX_SOCKET_H
#define PROXY_LINUX_SOCKET_H


#include <bits/sockaddr.h>
#include <cstdint>
#include <netinet/in.h>
#include <string>
#include "file_descriptor.h"
#include "server_utils.h"

class linux_socket {
public:
    linux_socket();

    linux_socket(int fd);


    linux_socket(linux_socket&&) = default;
    linux_socket& operator=(linux_socket&&) = default;

    void bind(sa_family_t sa_family, uint16_t port, in_addr_t s_addr);

    void listen();

    int accept();


    uint32_t get_flags();

    int get_available_bytes();

    void set_flags(uint32_t flags);

    file_descriptor &get_fd();

    ~linux_socket();

    size_t write(const std::string &msg);

    std::string read(size_t buffer_size);

private:
    file_descriptor fd;

};


#endif //PROXY_LINUX_SOCKET_H
