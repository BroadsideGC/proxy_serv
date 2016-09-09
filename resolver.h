//
// Created by bigz on 13.08.16.
//

#ifndef PROXY_RESOLVER_H
#define PROXY_RESOLVER_H


#include <cstdio>
#include <condition_variable>
#include <thread>
#include <queue>
#include "sys/eventfd.h"
#include "http.hpp"
#include "lru_cache.hpp"
#include "file_descriptor.h"

class resolver {
public:
    resolver(size_t thread_count = 2);
    ~resolver();

    void resolve();
    void send();
    void stop();

    void add_task(std::unique_ptr<http_request>);
    std::unique_ptr<http_request> get_task();

    file_descriptor& get_fd();
private:
    bool working = false;
    std::mutex lk;
    std::condition_variable condition;
    std::vector<std::thread> threads;
    std::queue<std::unique_ptr<http_request> > tasks, resolved;
    file_descriptor fd;

};


#endif //PROXY_RESOLVER_H
