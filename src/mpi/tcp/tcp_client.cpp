#include "print_macros.hpp"
#include "tcp.hpp"
#include <arpa/inet.h>  // sockaddr_in
#include <cstdio>       // perror
#include <sys/socket.h> // send, socket

tcp_client::tcp_client(int port, int host)
  : addr(port, host)
  , fd(socket(AF_INET, SOCK_STREAM, 0))
{}

tcp_client::~tcp_client() {}

void tcp_client ::dial()
{
    if (int ret = connect(fd, addr.get(), sizeof(sockaddr_in)); ret) {
        std::perror("connect failed");
    }
}

void tcp_client::send_exact(const char* p, int n)
{
    if (int sent = send(fd, p, n, 0); sent != n) {
        ERR("sent %d/%d", sent, n);
    }
}
