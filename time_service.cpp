
#include <iostream>
#include "time_service.h"

void time_service::add(timer *t) {
    pool.insert({t->time, t});
}

void time_service::erase(timer *t) {
    pool.erase(t->time);
}

timer::timer(time_service &service, steady_clock_t::duration duration, callback_t wakeuper)
        : service(&service),
          time(steady_clock_t::now() + duration),
          on_wake_up(std::move(wakeuper)) {
    service.add(this);
}

timer::~timer() {
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



















