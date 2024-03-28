#include <faabric/transport/tcp/Socket.h>

#include <atomic>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

namespace faabric::transport::tcp {
Socket::Socket()
  : connFd(socket(AF_INET, SOCK_STREAM, 0))
{
    if (get() <= 0) {
        std::perror("failed to create tcp_socket");
    }
}

Socket::Socket(int conn)
  : connFd(conn)
{}

int Socket::get()
{
    return connFd;
}
}
