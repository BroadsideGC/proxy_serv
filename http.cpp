

#include "http.hpp"
#include "server_utils.h"
#include <string>
#include <algorithm>

// http_protocol
http_protocol::http_protocol() : state(EMPTY) {}

http_protocol::http_protocol(std::string content) : state(EMPTY) {
    data = content;
}

void http_protocol::set_header(std::string key, std::string val) {
    std::cerr << "Key: " << key << "\n";
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    std::cerr << "Key: " << key << "\n";
    headers[key] = val;
}


std::string http_protocol::get_header(std::string header) {
    return headers.find(header) != headers.end() ? headers[header] : "";
}

void http_protocol::parse_data() {
    auto start_line_end = data.begin();
    if (state == EMPTY && data.find("\r\n") != std::string::npos) {
        auto first_space = std::find_if(data.begin(), data.end(), [](char a) { return a == ' '; });
        auto second_space = std::find_if(first_space + 1, data.end(), [](char a) { return a == ' '; });
        start_line_end = std::find_if(second_space + 1, data.end(), [](char a) { return a == '\r'; });
        if (first_space == data.end() || second_space == data.end() || start_line_end == data.end()) {
            state = BAD;
        } else {
            parse_start_line({data.begin(), start_line_end});
        }
    }

    size_t start_line_end_pos = std::distance(data.begin(), start_line_end);
    size_t _body_begin_pos = data.find("\r\n\r\n", start_line_end_pos);
    if (state == START_LINE && _body_begin_pos != std::string::npos) {
        std::string text_headers = data.substr(start_line_end_pos + 2, _body_begin_pos - start_line_end_pos);
        parse_headers(text_headers);
    }

    if (state == HEADERS || state == PARTIAL) {
        body_begin_pos = _body_begin_pos + 4;
        check_body();
    }
    /*if (state == BAD) {
        body_begin_pos = data.size() - 1;
    }*/
}

void http_protocol::parse_headers(std::string text_headers) {
    if (text_headers == "") {
        std::cerr << "No headers\n";
        state = BAD;
    } else {
        auto headers_end = text_headers.begin();

        while (headers_end != text_headers.end() && *headers_end != '\r') {
            auto space = std::find_if(headers_end, text_headers.end(), [](char a) { return a == ':'; });
            auto shift = 1;
            while (*(space + shift) == ' ')shift++;
            auto crlf = std::find_if(space + shift, text_headers.end(), [](char a) { return a == '\r'; });
            std::string cur_header = {headers_end, space};
            std::transform(cur_header.begin(), cur_header.end(), cur_header.begin(), ::tolower);
            headers.insert({cur_header, {space + shift, crlf}});
            headers_end = crlf + 2;
        };
        state = HEADERS;
    }
}

void http_protocol::check_body() {
    if (get_header("content-length") != "") {
        if (get_body().size() == static_cast<size_t>(std::stoi(get_header("content-length")))) {
            state = FULL;
        } else {
            state = PARTIAL;
        }
    } else if (get_header("transfer-encoding") == "chunked") {
        if (get_body().size() >= 5 && get_body().substr(get_body().size() - 5) == "0\r\n\r\n") {
            state = FULL;
        } else {
            state = PARTIAL;
        }
    } else {
        state = get_body().size() == 0 ? FULL : BAD;
    }
}

bool http_protocol::is_ended() {
    return state == FULL;
}

std::string http_protocol::get_data() {
    //std::string ret="";
    return get_start_line() + "\r\n" + get_headers() + "\r\n" + get_body();
}

std::string http_protocol::get_headers() {
    std::string headers_text = "";
    for (auto header : headers) {

        headers_text += header.first + ": " + header.second + "\r\n";
    }

    return headers_text;
}

std::string http_protocol::get_body() {
    return data.substr(body_begin_pos);
}

void http_protocol::append(std::string &content) {
    data.append(content);
    parse_data();
}


// http_request
http_request::http_request(std::string data) : http_protocol(data) {
    parse_data();
}

void http_request::parse_start_line(std::string start_line) {
    auto first_space = std::find_if(start_line.begin(), start_line.end(), [](char a) { return a == ' '; });
    auto second_space = std::find_if(first_space + 1, start_line.end(), [](char a) { return a == ' '; });
    auto crlf = std::find_if(second_space + 1, start_line.end(), [](char a) { return a == '\r'; });

    method = {start_line.begin(), first_space};
    URI = {first_space + 1, second_space};
    protocol = {second_space + 1, crlf};

    bool is_valid_method = method == "GET" || method == "POST" || method == "CONNECT";
    bool is_valid_URI = URI != "";
    bool is_valid_protocol = protocol == "HTTP/1.0" || protocol == "HTTP/1.1";

    state = (!is_valid_method || !is_valid_URI || !is_valid_protocol) ? BAD : START_LINE;
}

std::string http_request::get_start_line() {
    return method + " " + get_relative_URI() + " " + protocol;
}

std::string http_request::get_relative_URI() {
    std::string relative_URI = URI;
    if (relative_URI.substr(0, 7) == "http://") {
        relative_URI = relative_URI.substr(7);
    }
    if (relative_URI.substr(0, get_host().length()) == get_host()) {
        relative_URI = relative_URI.substr(get_host().length());
    }

    return relative_URI;
}

std::string http_request::get_port() {
    if (port.empty()) {
        get_host();
    }
    return port;
}

std::string http_request::get_host() {
    if (host == "") {
        if (get_header("host") == "") {
            host = port = "";
        } else {
            host = get_header("host");

            // "hostname.com:80" -> "hostname.com"
            port = (host.find(":") == std::string::npos) ? "80" : host.substr(host.find(":") + 1);
            host = (host.find(":") == std::string::npos) ? host : host.substr(0, host.find(":"));
        }
    }

    return host;
}

sockaddr http_request::get_resolved_host() {
    if (!is_host_resolved) {
        throw_server_error("Host didn't resolved!");
    }
    return resolved_host;
}

void http_request::set_resolved_host(sockaddr rh) {
    is_host_resolved = true;
    resolved_host = rh;
}

bool http_request::is_validating() {
    return get_header("if-match") != ""
           || get_header("if-modified-since") != ""
           || get_header("if-none-match") != ""
           || get_header("if-range") != ""
           || get_header("if-unmodified-since") != "";
}

// http_response
http_response::http_response() : http_protocol("") {}

http_response::http_response(std::string data) : http_protocol(data) {
    parse_data();
}

std::string http_response::get_start_line() {
    return protocol + " " + status;
}

void http_response::parse_start_line(std::string start_line) {
    auto first_space = std::find_if(start_line.begin(), start_line.end(), [](char a) { return a == ' '; });
    auto second_space = std::find_if(first_space + 1, start_line.end(), [](char a) { return a == ' '; });
    protocol = {start_line.begin(), first_space};
    status = {first_space + 1, second_space};
    //std::cout<<status << " " << protocol <<"\n";
    if (status <= "100" || status >= "999" || (protocol != "HTTP/1.0" && protocol != "HTTP/1.1")) {
        data = BAD_GETAWAY();
        parse_data();
        incorrect = true;
        return;
    }
    bool is_valid_status = status >= "100" && status <= "999";
    bool is_valid_protocol = protocol == "HTTP/1.0" || protocol == "HTTP/1.1";

    state = (!is_valid_status || !is_valid_protocol) ? BAD : START_LINE;
}

bool http_response::check_cache_control() {
    auto hdr = get_header("cache-control");
    if (hdr == "") {
        return true;
    }
    return hdr.find("private") == std::string::npos
           && hdr.find("no-cache") == std::string::npos
           && hdr.find("no-store") == std::string::npos;
}

bool http_response::is_cacheable() {
    return state == FULL && status == "200" && check_cache_control() && get_header("ETag") != "" && get_header("Vary") == "";
}
