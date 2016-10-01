

#include "server.h"

#include <cassert>

server::server(struct sockaddr addr, proxy_server &proxyServer, client &cl) : socket(linux_socket(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0))),
                                                                              event(io_event(proxyServer.get_epoll(), socket.get_fd(), EPOLLOUT, [this, &proxyServer](uint32_t events)mutable throw(std::runtime_error) {
                                                                                  try {
                                                                                      std::cerr << "Processing " << get_host() << " " << get_fd().get_fd() << "\n";
                                                                                      if (events & EPOLLIN) {
                                                                                          //std::cerr << "SERVER EPOLLIN\n";
                                                                                          read_response(proxyServer);
                                                                                      }
                                                                                      if (events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                                                                                          std::cerr << "Time to disconnect\n";
                                                                                          disconnect(proxyServer);
                                                                                          return;
                                                                                      }
                                                                                      if (events & EPOLLOUT) {
                                                                                          //std::cerr << "SERVER EPOLLOUT\n";
                                                                                          write_request(proxyServer);
                                                                                      }


                                                                                  } catch (std::runtime_error &e) {
                                                                                      std::cerr << "Error disc\n";
                                                                                      disconnect(proxyServer);
                                                                                  }
                                                                              })), time(proxyServer.get_epoll(), *this, proxyServer) {
    if (connect(socket.get_fd().get_fd(), &addr, sizeof(addr)) == -1) {
        if (errno != EINPROGRESS) {
            perror("Error while connecting to server occurred");
        }
    }
}

file_descriptor &server::get_fd() {
    return socket.get_fd();
}

file_descriptor &server::get_client_fd() {
    assert(paired_client);
    return paired_client->get_fd();
}

void server::set_host(const std::string &host) {
    this->host = host;
}

std::string server::get_host() {
    return host;
}


void server::append(std::string &data) {
    buffer.append(data);
}

std::string &server::get_buffer() {
    return buffer;
}

size_t server::get_buffer_size() {
    return buffer.size();
}

size_t server::write() {
    try {
        size_t written_cnt = socket.write(buffer);
        buffer.erase(0, written_cnt);
        if (paired_client) {
            this->push_to_server();
        }
        return written_cnt;
    } catch (...) {
        return 0;
    }
}

std::string server::read() {
    assert(paired_client);
    std::cerr << "Paired client " << paired_client->get_fd().get_fd() << "\n";
    try {
        std::string data = socket.read(socket.get_available_bytes());
        buffer.append(data);
        return data;
    } catch (...) {
        return "";
    }
}

void server::push_to_server() {
    assert(paired_client);
    paired_client->flush_client_buffer();
}


void server::push_to_client() {
    assert(paired_client);
    paired_client->get_buffer().append(buffer);
    buffer.clear();

}


server::~server() {
    //std::cerr << "Destroing server\n";
    //proxyServer.erase_server( get_fd().get_fd());
}

void server::disconnect(proxy_server &proxyServer) {
    //std::cerr << "Disconnect server " << get_fd().get_fd() << get_host() << "\n";
    int sfd = get_fd().get_fd();
    proxyServer.erase_server(sfd);
    if (paired_client->request_server.get() == this) {
        paired_client->unbind();
        //std::cerr<<"Test3\n";
    }
}

void server::read_response(proxy_server &proxyServer) {
    std::cerr << "Read data from server, fd = " << get_fd().get_fd() << "\n";
    if (socket.get_available_bytes() == 0) {
        event.remove_flag(EPOLLIN);
        return;
    }

    time.reset();

    std::string data = read();

    http_response *cur_response = paired_client->get_response();

    cur_response->append(data);

    //std::cerr << "State: " << cur_response->get_stat() << "\n";


    if (cur_response->get_stat() == http_response::BAD) {
        std::cerr << "Bad response " << get_host()<<"\n";
        std::cerr << "Response--------------------------------------------\n\n";
        std::cerr << cur_response->get_data();
        std::cerr << "----------------------------------------------------\n\n";
        buffer = http_protocol::BAD_REQUEST();
        push_to_client();
        paired_client->event.add_flag(EPOLLOUT);
        return;
    }
    ///std::cerr<<"Response:\n";
//
    //std::cerr<<"--------------------------------------------------------------------------------------------\n\n"<<cur_response->get_data()<<"\n\n";
    //std::cerr<<"--------------------------------------------------------------------------------------------\n\n";

    if (cur_response->is_ended()) {
        std::string cache_key = paired_client->get_request()->get_host() + paired_client->get_request()->get_relative_URI();
        // check cache hit
        std::cerr << cur_response->get_status() << '\n';
        if (cur_response->get_status() == "400") {
            std::cerr << "Request--------------------------------------------\n\n";
            std::cerr << request << "\n\n";
            std::cerr << "---------------------------------------------------\n\n";
        }
        if (cur_response->get_status() == "304" && proxyServer.get_cache().contains(cache_key)) {
            std::cerr << "Cache hit for URI " << cache_key << "\n";

            http_response cached_response = proxyServer.get_cache().get(cache_key);
            get_buffer() = cached_response.get_data();
        }

        // try cache
        if (cur_response->is_cacheable() && !proxyServer.get_cache().contains(cache_key)) {
            proxyServer.get_cache().put(cache_key, *cur_response);
            std::cerr << "Response added to cache for " << cache_key.c_str() << "\n";
        }

        push_to_client();
        paired_client->event.add_flag(EPOLLOUT);
        std::cerr << "Client " << get_client_fd().get_fd() << " must write\n";
    }
}

void server::write_request(proxy_server &proxyServer) {
    std::cerr << "Writing data to server, fd = " << get_fd().get_fd() << "\n";
    time.reset();

    int error;
    socklen_t length = sizeof(error);
    if (getsockopt(get_fd().get_fd(), SOL_SOCKET, SO_ERROR, &error, &length) == -1 || error != 0) {
        perror("Error while connecting to server. Disconnecting...");
        disconnect(proxyServer);
        return;
    }
    request = buffer;
    write();
    if (get_buffer_size() == 0) {
        event.add_flag(EPOLLIN);
        event.remove_flag(EPOLLOUT);
        //paired_client->event.add_flag(EPOLLOUT);
    }
}


void server::add_flag(uint32_t flag) {
    event.add_flag(flag);
}

void server::bind(client &new_client) {
    paired_client = &new_client;
}

void server::unbind() {
    paired_client = nullptr;
}

