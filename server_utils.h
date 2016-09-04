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



void throw_server_error(std::string msg);

#endif //PROXY_UTILS_H

