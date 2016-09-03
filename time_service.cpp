
#include <iostream>
#include "time_service.h"

void time_service::add(timer *t) {
    pool.insert({t->time, t});
}

void time_service::erase(timer *t) {
    pool.erase(t->time);
}

bool time_service::empty() const {
    return pool.empty();
}

steady_clock_t::time_point time_service::top() const {
    return pool.begin()->first;
}

int time_service::time_to_nearest_timeout() {
    if (!empty()) {
        steady_clock_t::time_point now(steady_clock_t::now());
        wake_up_timers(now);
        return empty() ? -1 : std::chrono::duration_cast<std::chrono::milliseconds>(top() - now).count();
    } else {
        return -1;
    }
}

void time_service::wake_up_timers(steady_clock_t::time_point point) {
    while (!empty()) {
        if (top() > point) {
            break;
        }

        auto cur_timer = pool.begin()->second;
        cur_timer->service = nullptr;

        try {
            cur_timer->on_wake_up();
        } catch (...) {
            //Can't wake up timer
        }

        pool.erase(pool.begin());
    }
}


timer::timer(time_service &service, steady_clock_t::duration duration, callback_t wakeuper)
        : service(&service),
          time(steady_clock_t::now() + duration),
          on_wake_up(std::move(wakeuper))
{
    service.add(this);
}

timer::~timer() {
    remove_from_poll();
}

void timer::stop() {
    remove_from_poll();
}

void timer::remove_from_poll() {
    if (service) {
        service->erase(this);
    }
}

void timer::change_time(steady_clock_t::duration duration) {
    if (service) {
        service->erase(this);
        time = steady_clock_t::now() + duration;
        service->add(this);
    } else {
        return;
    }
}



















