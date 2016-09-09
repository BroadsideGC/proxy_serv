
#ifndef PROXY_SERVER_TIME_SERVICE_H
#define PROXY_SERVER_TIME_SERVICE_H


#include <chrono>
#include <functional>
#include <map>

const std::chrono::seconds SOCKET_TIMEOUT = std::chrono::seconds(3);
const std::chrono::seconds CONNECTION_TIMEOUT = std::chrono::seconds(1);
typedef std::chrono::steady_clock steady_clock_t;
typedef std::function<void()> callback_t;

class timer;

class time_service {
public:
    time_service() {};

    void add(timer *t);

    void erase(timer *t);

private:
    std::map<steady_clock_t::time_point, timer *> pool;
};

class timer {
    friend class time_service;

public:
    timer(time_service &service, steady_clock_t::duration duration, callback_t);

    ~timer();

    void change_time(steady_clock_t::duration duration);

private:
    time_service *service;
    callback_t on_wake_up;
    steady_clock_t::time_point time;

    void remove_from_poll();
};

#endif //PROXY_SERVER_TIME_SERVICE_H
