#ifndef PROXY_SERVER_FILE_DESCRIPTOR_H
#define PROXY_SERVER_FILE_DESCRIPTOR_H

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <cstdint>
#include "server_utils.h"
#include "string.h"


struct file_descriptor {

public:

    file_descriptor &operator=(const file_descriptor &rhs) = delete;

    file_descriptor(const file_descriptor &) = delete;


    file_descriptor();

    file_descriptor(int fd);

    file_descriptor(file_descriptor &&fd);

    file_descriptor &operator=(file_descriptor &&rhs);

    ~file_descriptor();


    uint32_t get_flags();

    void set_flags(uint32_t nex_flags);

    int get_available_bytes();

    void make_nonblocking();

    void close();

    int &get_fd();

private:
    int fd;

};


#endif //PROXY_SERVER_FILE_DESCRIPTOR_H
