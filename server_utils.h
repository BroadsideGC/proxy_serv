//
// Created by bigz on 04.08.16.
//


#ifndef PROXY_UTILS_H
#define PROXY_UTILS_H


#include <string>
#include <stdexcept>
#include <vector>
#include <mutex>
#include <iostream>
#include <execinfo.h>
#include <stdlib.h>

const std::chrono::seconds SOCKET_TIMEOUT = std::chrono::seconds(600);
const std::chrono::seconds CONNECTION_TIMEOUT = std::chrono::seconds(180);

void throw_server_error(std::string msg);

class Log {
private:
    Log() = delete;

    static int level;
    static std::vector<std::ostream *> targets;

    static std::string get_time() {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        return buf;
    }

    static std::mutex mutex;

    static void print(std::string prefix, std::string message) {
        std::string result_message = get_time() + " [" + prefix + "]: " + message;

        std::unique_lock<std::mutex> lk(mutex);
        fflush(stdout);
        fflush(stderr);
        std::cerr.flush();
        std::cout.flush();
        for (const auto &o : targets) {
            o->flush();
            (*o) << result_message << std::endl;
            o->flush();
        }
        std::cerr.flush();
        std::cout.flush();
        fflush(stdout);
        fflush(stderr);
    }

public:
    static void add_output(std::ostream *o) {
        targets.push_back(o);
    }

    static void set_level(int level) {
        Log::level = level;
    }

    static void e(std::string message) {
        if (level < 0) return;
        print("E", message);
    }

    static void w(std::string message) {
        if (level < 1) return;
        print("W", message);
    }

    static void d(std::string message) {
        if (level < 2) return;
        print("D", message);
    }

    static void fatal(std::string message) {
        e(message);
        throw std::runtime_error(message);
    }

    static void print_stack_trace(int depth) {
        fflush(stdout);
        fflush(stderr);

        void *trace[depth];
        int trace_size = backtrace(trace, depth);

        //trace[1] = (void *) ctx.eip;
        char **messages = backtrace_symbols(trace, trace_size);
        /* skip first stack frame (points here) */
        printf("Execution path [thread: %d]:\n", (int) pthread_self());
        for (int i = 1; i < trace_size; ++i) {
            //printf("[bt] #%d %s\n", i, messages[i]);

            /* find first occurence of '(' or ' ' in message[i] and assume
             * everything before that is the file name. (Don't go beyond 0 though
             * (string terminator)*/
            size_t p = 0;
            while (messages[i][p] != '(' && messages[i][p] != ' ' && messages[i][p] != 0)
                ++p;

            char syscom[256];
            sprintf(syscom, "addr2line %p -e %.*s", trace[i], (int)p, messages[i]);
            //last parameter is the file name of the symbol
            system(syscom);
        }
        free(messages);

        fflush(stdout);
        fflush(stderr);
    }
};

#endif //PROXY_UTILS_H

