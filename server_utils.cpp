
//
// Created by bigz on 04.08.16.
//

#include "server_utils.h"

void throw_server_error(std::string msg) {
    throw std::runtime_error(msg);
}
