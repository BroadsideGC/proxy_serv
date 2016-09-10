//
// Created by bigz on 03.08.16.
//

#include <vector>
#include <csignal>
#include <iostream>
#include "linux_socket.h"


linux_socket::linux_socket() : fd(-1) {

}


linux_socket::linux_socket(int fd) : fd(fd) {
    set_flags(get_flags() | SOCK_STREAM | SOCK_NONBLOCK);
}




void linux_socket::bind(sa_family_t sa_family, uint16_t port, in_addr_t s_addr) {
    struct sockaddr_in hints;

    hints.sin_family = sa_family;
    hints.sin_port = htons(port);
    hints.sin_addr.s_addr = htons(s_addr);

    if (::bind(fd.get_fd(), (struct sockaddr *) &hints, sizeof(hints)) == -1) {

        throw_server_error("Error in bind posix_socket");
    }
}

void linux_socket::listen() {
    if (::listen(fd.get_fd(), SOMAXCONN) == -1) {
        throw_server_error("Error in listen posix_socket");
    }
}

int linux_socket::accept() {
    int result = ::accept(fd.get_fd(), (struct sockaddr *) NULL, NULL);
    if (result == -1) {
        throw_server_error("Error in accept posix_socket");
    }

    return result;
}

uint32_t linux_socket::get_flags() {
    return fd.get_flags();
}

int linux_socket::get_available_bytes() {
    return fd.get_available_bytes();
}


void linux_socket::set_flags(uint32_t flags) {
    fd.set_flags(flags);
}

file_descriptor &linux_socket::get_fd() {
    return fd;
}

linux_socket::~linux_socket() {
    //std::cerr << "Socket destroyed\n";
}


size_t linux_socket::write(std::string const &msg) {
    ssize_t len;
    if ((len = send(get_fd().get_fd(), msg.c_str(), msg.size(), 0)) == -1) {
        throw_server_error("Error while writing!");
    }

    return len;
}

std::string linux_socket::read(size_t buffer_size) {
    std::vector<char> buffer(buffer_size);

    if ((recv(get_fd().get_fd(), buffer.data(), buffer_size, 0)) == -1) {
        throw_server_error("Error while reading!");
    }

    return std::string(buffer.begin(), buffer.end());
}

