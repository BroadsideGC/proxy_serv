//
// Created by bigz on 10.08.16.
//

#ifndef PROXY_EVENT_H
#define PROXY_EVENT_H


#include "file_descriptor.h"
#include "epoll_io.h"

class epoll_io;

class io_event {
    friend class epoll_io;

public:


    io_event(epoll_io &service, file_descriptor &fd, uint32_t flags, std::function<void(uint32_t)> handler);


    ~io_event();

    file_descriptor &get_fd() {
        return fd;
    }

    void add_flag(uint32_t flag);

    void remove_flag(uint32_t flag);

    std::function<void(uint32_t)> callback;

private:
    uint32_t flags;
    epoll_io &service;
    file_descriptor &fd;
};


#endif //PROXY_EVENT_H
