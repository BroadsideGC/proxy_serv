//
// Created by bigz on 10.08.16.
//

#include <sys/epoll.h>
#include "io_event.h"

io_event::io_event(epoll_io &service, file_descriptor &fd, uint32_t flags, std::function<void(uint32_t)> handler)
        : service(service), fd(fd), flags(flags | EPOLLERR | EPOLLRDHUP | EPOLLHUP), callback(handler) {
    service.add(fd, *this, flags);

}

io_event::~io_event() {
    //std::cerr<<"Removing from epoll "<<fd.get_fd()<<"\n";
    service.remove(fd, *this, 0);
   // std::cerr<<"Event deleted\n";
}


void io_event::add_flag(uint32_t flag) {
    flags |= flag;
    service.modify(fd, *this, flags);
}

void io_event::remove_flag(uint32_t flag) {
    flags &= ~flag;
    service.modify(fd, *this, flags);
}

