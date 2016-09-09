//
// Created by bigz on 04.08.16.
//

#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H


#include <cstddef>
#include <string>
#include <memory>
#include "linux_socket.h"
#include "server.h"
#include "http.hpp"
#include "io_event.h"
#include <queue>
#include "proxy_server.h"
#include "timer.h"

class proxy_server;

class client {
    friend class proxy_server;
    friend class server;
    friend class timer<client>;
public:
    client(const client &) = delete;

    client &operator=(client const &) = delete;

    client(int fd, proxy_server &proxy);

    ~client();

    file_descriptor& get_fd();

    file_descriptor& get_server_fd();

    bool has_server();

    void bind(server &new_server);

    void unbind();

    std::string &get_buffer();


    size_t get_buffer_size();

    size_t read();

    size_t write();

    void flush_server_buffer();

    void flush_client_buffer();

    std::string get_host();

    void set_response(http_response *new_response);

    http_response * get_response();

    void set_request(http_request *new_request);

    http_request * get_request();

private:
    void read_request(proxy_server &proxyServer);
    void write_response(proxy_server &);
    void disconnect(proxy_server &proxyServer);
    std::string buffer;
    linux_socket socket;
    io_event event;
    timer<client> time;
    std::unique_ptr<server> request_server;
    std::unique_ptr<http_response> response;
    std::unique_ptr<http_request> request;
};


#endif //PROXY_CLIENT_H
