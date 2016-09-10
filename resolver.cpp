//
// Created by bigz on 13.08.16.
//

#include <csignal>
#include "resolver.h"

resolver::resolver(size_t thread_count) {
    working = true;
    fd = eventfd(0, EFD_NONBLOCK|EFD_SEMAPHORE);
    try {
        for (auto i = 0; i < thread_count; i++) {
            threads.push_back(std::thread([this]() {
                this->resolve();
            }));
        };
        std::cerr << "Resolver started with " << thread_count << " threads\n";
    } catch (...) {
        working = false;
        for (auto i = 0; i < threads.size(); i++) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
        throw;
    }
}

resolver::~resolver() {
    std::cerr << "Stoping resolver\n";
    stop();
}

void resolver::resolve() {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, nullptr);
    while (working) {

        std::unique_lock<std::mutex> lock{lk};
        condition.wait(lock, [&]() {
            return (!tasks.empty() || !working);
        });


        if (!working) {
            return;
        }
        std::cerr << "In resolver\n";
        auto request = std::move(tasks.front());
        tasks.pop();

        lock.unlock();
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));

        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        int err_no = getaddrinfo(request->get_host().c_str(), request->get_port().c_str(), &hints, &res);

        sockaddr host;

        if (err_no != 0) {
            perror("Resolver: error while resolving");
        } else {
            host = *res->ai_addr;
            freeaddrinfo(res);
            lock.lock();
            lock.unlock();
        }
        request->set_resolved_host(host);
        std::cerr << "Host resolved! " << request->get_host().c_str() << "\n";

        lock.lock();
        resolved.push(std::move(request));
        lock.unlock();

        send();
        std::cerr << "Resolve ended\n";
    }
}


void resolver::send() {
    std::unique_lock<std::mutex> lock{lk};
    if (eventfd_write(fd.get_fd(), 1) == -1) {
        lock.unlock();
        perror("Resolver: error while sending message to proxy server");
    }
}

void resolver::stop() {
    std::unique_lock<std::mutex> lock{lk};
    working = false;
    condition.notify_all();
    lock.unlock();
    for (auto i = 0; i < threads.size(); i++) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
}

void resolver::add_task(std::unique_ptr<http_request> request) {
    if (!working) {
        throw_server_error("Resolver doesn't running!");
    }
    std::unique_lock<std::mutex> lock{lk};
    tasks.push(std::move(request));
    condition.notify_one();
}

std::unique_ptr<http_request> resolver::get_task() {
    std::unique_lock<std::mutex> lock{lk};
    auto request = std::move(resolved.front());
    resolved.pop();
    return request;
}

file_descriptor &resolver::get_fd() {
    return fd;
}

