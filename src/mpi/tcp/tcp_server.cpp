#include "print_macros.hpp"
#include "tcp.hpp"
#include <arpa/inet.h>  // sockaddr_in
#include <cstdio>       // perror
#include <sys/socket.h> // bind, listen, recv, socket
#include <unistd.h>     // close

void handle_conn(int fd)
{
    PRN("%s: %d", __func__, fd);
    char data[1 << 20];
    int limit = sizeof(data);
    for (;;) {
        int n = recv(fd, data, limit, 0);
        PRN("got %d/%d", n, limit);
        data[n] = '\0';
        PRN("content: %s", data);
        if (n == 0) {
            break;
        }
    }
}

tcp_server::tcp_server(int port, int host)
  : addr(port, host)
  , fd(socket(AF_INET, SOCK_STREAM, 0))
  , stopped(false)
{}

tcp_server::~tcp_server()
{
    close(fd);
}

void tcp_server::start()
{
    if (int ret = bind(fd, addr.get(), sizeof(sockaddr_in)); ret) {
        std::perror("bind failed");
        return;
    }
    if (int code = listen(fd, 1024); code) {
        std::perror("listen failed");
        return;
    }
    for (; !stopped;) {
        if (int conn = accept(fd, nullptr, nullptr); conn < 0) {
            std::perror("accept failed");
            break;
        } else {
            PRN("accept: %d", conn);
            handle_conn(conn);
            PRN("handled: %d", conn);
        }
    }
}

void tcp_server::stop()
{
    stopped = true;
}
