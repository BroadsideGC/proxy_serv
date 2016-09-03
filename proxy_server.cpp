//
// Created by bigz on 04.08.16.
//

#include <iostream>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <csignal>
#include "proxy_server.h"

proxy_server::proxy_server(int port) : main_socket(::socket(AF_INET, SOCK_STREAM, 0)),
                                       rslvr(sysconf(_SC_NPROCESSORS_ONLN)) {
    std::cout << epoll.get_fd() << '\n';
    int one = 1;
    setsockopt(main_socket.get_fd().get_fd(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    main_socket.bind(AF_INET, port, htons(INADDR_ANY));
    main_socket.listen();
    main_socket.get_fd().make_nonblocking();
    std::cout << "Listening started\n";
    main_socket.set_flags(main_socket.get_flags() | EPOLLIN);
    int fd[2];
    pipe(fd);
    pipe_fd = file_descriptor(fd[0]);
    file_descriptor resolver_fd(fd[1]);

    pipe_fd.make_nonblocking();
    resolver_fd.make_nonblocking();
    rslvr.set_fd(std::move(resolver_fd));
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    std::cout << "Resolver fds: " << rslvr.get_fd().get_fd() << " " << pipe_fd.get_fd() << "\n";
    resolver_event = std::make_unique<io_event>(epoll, pipe_fd, EPOLLIN,
                                                [this](uint32_t events)mutable throw(std::runtime_error) {
                                                    std::cout << "Resolver handler\n";
                                                    this->resolver_handler(events);
                                                });
    listen_event = std::unique_ptr<io_event>(new io_event(epoll, main_socket.get_fd(), EPOLLIN,
                                                          [this](uint32_t events)mutable throw(std::runtime_error) {
                                                              connect_client(events);
                                                          }));
    printf("Main listener(%d) added to epoll!\n", main_socket.get_fd().get_fd());
}

proxy_server::~proxy_server() {
    fprintf(stdout, "Server stopped.\n");
    rslvr.stop();
}

void proxy_server::run() {
    std::cout << "Server started\n";
    epoll.run();
}


void proxy_server::terminate() {

}

void proxy_server::connect_client(uint32_t) {
    auto client_fd = this->main_socket.accept();
    printf("Socket assigned to:%d \n", client_fd);
    clients[client_fd] = std::make_unique<client>(client_fd, this);
    std::cout << clients.size() << " clients now\n";
}




epoll_io &proxy_server::get_epoll() {
    return epoll;
}

void proxy_server::resolver_handler(uint32_t events) {
    char tmp;
    if (read(pipe_fd.get_fd(), &tmp, sizeof(tmp)) == -1) {
        perror("Reading from resolver failed");
    }

    std::unique_ptr<http_request> cur_request = rslvr.get_task();
    fprintf(stdout, "Resolver callback called for host [%s].\n", cur_request->get_host().c_str());

    client *cur_client = clients[cur_request->get_client_fd()].get();

    if (cur_client == nullptr) {
        return;
    }

    server *srvr;

    try {
        struct sockaddr result = cur_request->get_resolved_host();
        srvr = new server(result, this, cur_client);
    } catch (...) {
        throw_server_error("Error while connecting to server!");
    }

    cur_client->time.change_time(SOCKET_TIMEOUT);

    srvr->set_host(cur_request->get_host());
    cur_client->bind(srvr);
    fprintf(stdout, "Server with fd = %d binded to client with fd = %d\n", srvr->get_fd().get_fd(),
            cur_client->get_fd().get_fd());
    srvr->add_flag(EPOLLOUT);
    servers[srvr->get_fd().get_fd()] = srvr;
    std::cout << servers.size() << " servers now\n";
    cur_client->get_buffer() = std::move(cur_request->get_data());
    cur_client->flush_client_buffer();
}

void proxy_server::erase_server(int fd) {
    fprintf(stdout, "Erasing server, fd = %lu, host = [%s]\n", fd, servers[fd]->get_host().c_str());
    servers.erase(fd);
    std::cout << servers.size() << " servers left\n";
}


void proxy_server::add_task(std::unique_ptr<http_request> request) {
    rslvr.add_task(std::move(request));
}

void proxy_server::erase_client(int fd) {
    std::cout << "Erasing client " << "\n";
    clients.erase(fd);
    std::cout << clients.size() << " clients left\n";
}

bool proxy_server::find_client(int fd) {
    return clients.find(fd) != clients.end();
}

size_t proxy_server::get_num_of_clients() {
    return clients.size();
}

lru_cache<std::string, http_response> &proxy_server::get_cache() {
    return cache;
}
