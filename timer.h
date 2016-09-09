//
// Created by bigz on 09.09.16.
//

#ifndef PROXY_TIMER_H
#define PROXY_TIMER_H


#include <sys/epoll.h>
#include <bits/signum.h>
#include "io_event.h"
#include "sys/timerfd.h"
#include "server_utils.h"
#include "client.h"
#include "server.h"
#include "proxy_server.h"

#define TIMEOUT 180

class proxy_server;
class client;

class server;

template<typename type_t>
class timer {
public:

    timer(epoll_io &epp, type_t &side, proxy_server &proxyServer) : side(side), fd(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)), event(epp, fd.get_fd(), EPOLLIN, [this, &side, &proxyServer](uint32_t) {
        std::cerr << "Timer Timeout\n";
        char buf[8];
        read(fd.get_fd().get_fd(), buf, 8);
        side.disconnect(proxyServer);
        return;
    }) {
        reset();
        std::cerr << "Created timer with fd " << event.get_fd().get_fd() << "\n";
    }

    ~timer() {
       // std::cerr<<"Remove timer\n";
    }

    void reset() {
        struct itimerspec timeout;
        /* set timeout */
        timeout.it_value.tv_sec = TIMEOUT;
        timeout.it_value.tv_nsec = 0;
        timeout.it_interval.tv_sec = TIMEOUT;
        /* recurring */
        timeout.it_interval.tv_nsec = 0;
        auto ret = timerfd_settime(fd.get_fd().get_fd(), 0, &timeout, NULL);
        //std::cerr << "Timeout set " << TIMEOUT << '\n';
        if (ret) {
            throw_server_error("Failed to set timeout");
        }
    };

private:
    type_t &side;
    linux_socket fd;
    io_event event;
};


#endif //PROXY_TIMER_H
