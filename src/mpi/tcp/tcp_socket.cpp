#include <cstdio> // perror

#include <atomic> // atomic

#include <log.hpp>
#include <print_macros.hpp>
#include <sys/socket.h> // socket
#include <tcp.hpp>
#include <unistd.h> // close

file_descriptor::file_descriptor(int n)
{
    int *p = new int;
    *p = n;
    fd.reset(p);
}

file_descriptor::operator int()
{
    return *fd;
}

void fd_deleter::operator()(int *fd)
{
    close(*fd);
    // LOG("fd %d closed", *fd);
    delete fd;
}

tcp_socket::tcp_socket(std::atomic<bool> *stopped)
    : fd(socket(AF_INET, SOCK_STREAM, 0)), stopped(stopped)
{
    if (get() <= 0) {
        std::perror("failed to create tcp_socket");
    }
}

tcp_socket::tcp_socket(int conn, std::atomic<bool> *stopped)
    : fd(conn), stopped(stopped)
{
    // LOG("%s(%d) created", __func__, (int)fd);
}

int tcp_socket::get()
{
    return fd;
}
