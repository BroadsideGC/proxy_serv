

#ifndef http_hpp
#define http_hpp

#include <stdio.h>
#include <string>
#include <netdb.h>
#include <unordered_map>


class http_protocol {

public:

    enum state_t {
        FULL, BAD, START_LINE, HEADERS, EMPTY, PARTIAL
    };


    static std::string BAD_REQUEST() {
        std::string request = std::string("HTTP/1.1 400 Bad Request\r\n");
        request.append("Server: lol\r\n");
        request.append("Content-Type: text/html; charset=utf-8\r\n");
        request.append("Content-Length: 163\r\n");
        request.append("Connection: close\r\n\r\n");
        request.append("<html>\r\n");
        request.append("<head><title>400 Bad Request</title></head>\r\n");
        request.append("<body bgcolor=\"white\">\r\n");
        request.append("<center><h1>400 Bad Request</h1></center>\r\n");
        request.append("<hr><center>proxy</center>\r\n");
        request.append("</body>\r\n");
        request.append("</html>");

        return request;
    }

    static std::string BAD_GETAWAY() {
        std::string request = std::string("HTTP/1.1 502 Bad Getaway\r\n");
        request.append("Server: lol\r\n");
        request.append("Content-Type: text/html; charset=utf-8\r\n");
        request.append("Content-Length: 163\r\n");
        request.append("Connection: close\r\n\r\n");
        request.append("<html>\r\n");
        request.append("<head><title>502 Bad Getaway</title></head>\r\n");
        request.append("<body bgcolor=\"white\">\r\n");
        request.append("<center><h1>502 Bad Getaway</h1></center>\r\n");
        request.append("<hr><center>proxy</center>\r\n");
        request.append("</body>\r\n");
        request.append("</html>");

        return request;
    }
    http_protocol();

    http_protocol(std::string);

    virtual ~http_protocol() {}

    std::string get_data();

    state_t get_stat() {
        return state;
    }

    void set_header(std::string key, std::string val);

    std::string get_header(std::string key);

    void append(std::string &);

    bool is_ended();

    std::string get(){
        return data;
    }

protected:
    void parse_data();

    virtual void parse_start_line(std::string) = 0;

    void parse_headers(std::string);

    void check_body();

    virtual std::string get_start_line() = 0;

    std::string protocol, data;

    std::string get_headers();

    std::string get_body();

    state_t state;

    bool incorrect = 0;

    size_t body_begin_pos;
    std::unordered_map<std::string, std::string> headers;
};

class http_request : public http_protocol {
public:

    http_request(std::string);

    std::string get_host();

    std::string get_port();

    sockaddr get_resolved_host();

    void set_resolved_host(sockaddr rh);

    std::string get_relative_URI();

    bool is_validating();

    int get_client_fd() {
        return client_fd;
    }

    void set_client_fd(int fd) {
        client_fd = fd;
    }

private:
    void parse_start_line(std::string) override;

    std::string get_start_line() override;

    std::string port = "", host = "", URI, method;
    sockaddr resolved_host;
    bool is_host_resolved = false;
    int client_fd;
};

class http_response : public http_protocol {
public:
    http_response();

    http_response(std::string);

    bool check_cache_control();

    bool is_cacheable();

    std::string get_status() {
        return status;
    }

private:
    void parse_start_line(std::string) override;

    std::string get_start_line() override;

    std::string status;
};

#endif /* http_hpp */
