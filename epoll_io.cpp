//
// Created by bigz on 06.08.16.
//

#include <sys/epoll.h>
#include "epoll_io.h"
#include <csignal>
#include "sys/signalfd.h"

epoll_io::epoll_io() {
    epoll_fd = ::epoll_create(MAX_EPOLL_EVENTS_COUNT);
    if (epoll_fd == -1) {
        throw_server_error("Error epoll_create()");
    }

}


void epoll_io::run() {

    working = true;
    file_descriptor signal_fd = create_signal_fd();
    io_event signal_event(*this, signal_fd, EPOLLIN, [this](uint32_t) {
        perror("Signal caught\n");
        this->working = false;
    });
    epoll_event events[MAX_EPOLL_EVENTS_COUNT];
    std::cerr << "Еполл крутится, клиенты мутятся\n";
    while (working) {
        //printf("Щя послушаем\n");
        auto events_count = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS_COUNT, DEFAULT_EPOLL_TIMEOUT);
        if (events_count < 0) {
            if (errno != EINTR) {
                throw_server_error("Error in epoll_wait");
            } else {
                break;
            }
        }
        for (auto i = 0; i < events_count; i++) {
            auto &ev = events[i];
            io_event *x = (io_event *) (ev.data.ptr);
            if (available.find(x) != available.end()) {
                //std::cerr<<"Callback in\n";
                x->callback(ev.events);
                //std::cerr<<"Callback out\n";
            } else {
                std::cerr << "Io_event " << x << " is dead\n";
            }
        }
    }

}

void epoll_io::add(file_descriptor &fd, io_event &event, uint32_t flags) {
    available.insert(&event);
    operate(EPOLL_CTL_ADD, fd.get_fd(), event, flags);

}

void epoll_io::remove(file_descriptor &fd, io_event &event, uint32_t flags) {
    available.erase(&event);
    operate(EPOLL_CTL_DEL, fd.get_fd(), event, 0);

}

void epoll_io::modify(file_descriptor &fd, io_event &event, uint32_t flags) {
    operate(EPOLL_CTL_MOD, fd.get_fd(), event, flags);

}

epoll_io::~epoll_io() {
    //std::cerr<<"Destroing epoll\n";
    close(epoll_fd);
    // available.clear();
    std::cerr << "No more epoll\n";
}


void epoll_io::operate(int op, int fd, io_event &event, uint32_t flags) {
    struct epoll_event e_event;

    e_event.data.ptr = &event;
    e_event.events = flags;

    if (epoll_ctl(epoll_fd, op, fd, &e_event) == -1) {
        //std::cerr << "Epoll fd: " << epoll_fd << "\n";
        throw_server_error("Error in during operation on epoll_io event " + std::to_string(fd));
    }

}


int &epoll_io::get_fd() {
    return epoll_fd;
}


file_descriptor epoll_io::create_signal_fd() {
    sigset_t mask;
    sigemptyset(&mask);
    if (sigaddset(&mask, SIGINT) == -1) {
        std::cerr << "Not valid signal to block" << SIGINT << "\n";
    }
    if (sigaddset(&mask, SIGTERM) == -1) {
        std::cerr << "Not valid signal to block" << SIGTERM << "\n";
    }

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        throw_server_error("Error in sigprocmask");
    }

    int signal_fd = signalfd(-1, &mask, SFD_CLOEXEC | SFD_NONBLOCK);
    if (signal_fd == -1) {
        throw_server_error("Error in create of signal_fd");
    }
    std::cerr << "Signal fd created\n";
    return file_descriptor(signal_fd);
}


