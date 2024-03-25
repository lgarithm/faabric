#include <atomic> // atomic

#include "tcp.hpp"
#include <sys/socket.h> // socket
#include <unistd.h>     // close

tcp_socket::tcp_socket(std::atomic<bool>* stopped)
  : fd(socket(AF_INET, SOCK_STREAM, 0))
  , stopped(stopped)
{}

tcp_socket::tcp_socket(int conn, std::atomic<bool>* stopped)
  : fd(conn)
  , stopped(stopped)
{}

tcp_socket::~tcp_socket()
{
    close(fd);
}

int tcp_socket::get()
{
    return fd;
}
