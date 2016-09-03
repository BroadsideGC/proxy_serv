//
// Created by bigz on 02.08.16.
//

#include "file_descriptor.h"
#include "string.h"

file_descriptor::file_descriptor () : fd (-1) {

}



file_descriptor::file_descriptor (int fd) : fd (fd) {
   // std::cout<<"Created fd "<< fd<<"\n";
}

file_descriptor::~file_descriptor () {
    //std::cout<<"Fd "<<fd<<" destroyed\n";
    close ();
}

ssize_t file_descriptor::write_some (char *buffer, size_t size) {
    ssize_t res = ::write (fd, buffer, size);
    if (res == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            throw_server_error("Error in write_some");
        }
    }
    return res;
}

void file_descriptor::make_nonblocking () {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
        throw_server_error("Error while making nonblocking!");
    }
}

void file_descriptor::close () {
    if (fd == -1) {
        return;
    }
    if (::close (fd) == -1) {
        if (errno != EAGAIN) {
            throw_server_error("Error during closing file descriptor: " +std::to_string(fd));
        }
    }

}

int &file_descriptor::get_fd () {
    return fd;
}


int file_descriptor::get_flags () {
    auto result = fcntl (fd, F_GETFD);
    if (result == -1) {
        throw_server_error("Error during getting");
    }

    return result;
}

void file_descriptor::set_flags (uint32_t flags) {
    if (fcntl (fd, F_SETFD, flags) == -1) {
        throw_server_error("Error during setting flags");
    }
}

int file_descriptor::get_available_bytes () {
    int avail_bytes;
    if (ioctl (fd, FIONREAD, &avail_bytes) == 1) {
        throw_server_error("Error during getting availible bytes");
    }
    return avail_bytes;
}

file_descriptor::file_descriptor(file_descriptor &&fd) : fd(fd.fd){
    fd.fd=-1;
}

file_descriptor& file_descriptor::operator=(file_descriptor&& rhs) {
    fd = rhs.fd;
    rhs.fd = -1;

    return *this;
}
