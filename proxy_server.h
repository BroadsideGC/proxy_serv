//
// Created by bigz on 04.08.16.
//

#ifndef PROXY_PROXY_SERVER_H
#define PROXY_PROXY_SERVER_H



#include <map>
#include "file_descriptor.h"
#include "linux_socket.h"
#include "epoll_io.h"
#include "client.h"
#include "resolver.h"
#include "arpa/inet.h"
class client;
class server;
class proxy_server {
public:
    proxy_server(int port);

    proxy_server(proxy_server const& rhs) = delete;
    proxy_server& operator=(proxy_server const& rhs) = delete;

    ~proxy_server();

    epoll_io& get_epoll();

    void run();
    void erase_server(int fd);
    void erase_client(int fd);
    void add_task(std::unique_ptr<http_request> request);
    lru_cache<std::string, http_response>& get_cache();


private:

    void connect_client();
    void resolver_handler();

    epoll_io epoll;
    bool working;
    linux_socket main_socket;
    std::map<uintptr_t, std::unique_ptr<client> > clients;
    std::map<uintptr_t, server* > servers;
    lru_cache<std::string, http_response> cache;
    file_descriptor pipe_fd;
    class resolver rslvr;
    std::unique_ptr<io_event> listen_event;
    std::unique_ptr<io_event> resolver_event;
};


#endif //PROXY_PROXY_SERVER_H
