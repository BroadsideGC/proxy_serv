//
// Created by bigz on 06.08.16.
//

#ifndef PROXY_EPOLL_H
#define PROXY_EPOLL_H


#include <set>
#include <memory>
#include "file_descriptor.h"
#include "io_event.h"
#include "time_service.h"

const int DEFAULT_EPOLL_TIMEOUT = 10000;
const int MAX_EPOLL_EVENTS_COUNT = 1000;


class io_event;

class epoll_io {
    friend class io_event;

public:
    epoll_io();

    epoll_io(epoll_io const &) = delete;

    ~epoll_io();

    int &get_fd();

    void run();

    void add(file_descriptor &fd, io_event *event, uint32_t flags);

    void remove(file_descriptor &fd, io_event *event, uint32_t flags);

    void modify(file_descriptor &fd, io_event *event, uint32_t flags);

    time_service &get_time_service();

private:
    file_descriptor create_signal_fd(std::vector<uint8_t> signals);

    void operate(int op, int fd, io_event *event, uint32_t flags);

    bool working;
    int epoll_fd;
    std::set<io_event *> available;

    time_service timeService;

};


#endif //PROXY_EPOLL_H
