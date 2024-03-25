#include <cstdio> // perror

#include <memory> // make_unique, unique_ptr

#include "print_macros.hpp"
#include "tcp.hpp"
#include <arpa/inet.h>  // sockaddr_in
#include <sys/socket.h> // bind, listen, recv
#include <unistd.h>     // close

void handle_conn(std::unique_ptr<tcp_socket> conn)
{
    int fd = conn->get();
    PRN("%s: %d", __func__, fd);
    char data[1 << 20];
    int limit = sizeof(data);
    for (;;) {
        int n = recv(fd, data, limit, 0);
        PRN("got %d/%d", n, limit);
        if (n == 0) {
            break;
        }
    }
}

tcp_server::tcp_server(int port, int host)
  : addr(port, host)
  , stopped(false)
  , handling(0)
  , sock(&stopped)
{}

void tcp_server::serve(tcp_handler f)
{
    int fd = sock.get();
    if (int ret = bind(fd, addr.get(), sizeof(sockaddr_in)); ret) {
        std::perror("bind failed");
        return;
    }
    if (int code = listen(fd, 1024); code) {
        std::perror("listen failed");
        return;
    }
    // for (; !stopped;) {
    //     sockaddr_in remote;
    //     socklen_t addr_len;
    //     if (int conn = accept(fd, (sockaddr *)&remote, &addr_len); conn > 0)
    //     {
    //         PRN("accept: %d, addr_len: %d", conn, addr_len);
    //         show_addr(remote);
    //         f(conn);
    //         PRN("handled: %d", conn);
    //     } else {
    //         std::perror("accept failed");
    //         break;
    //     }
    // }

    for (; !stopped;) {
        if (int conn = accept(fd, nullptr, nullptr); conn > 0) {
            PRN("accept: %d", conn);
            f(std::make_unique<tcp_socket>(conn));
            // [&]() {
            //     ++handling;
            //     f(conn);
            //     --handling;
            // }();
            PRN("handled: %d", conn);
        } else {
            std::perror("accept failed");
            break;
        }
    }
}

void tcp_server::start()
{
    serve(handle_conn);
}

void tcp_server::stop()
{
    stopped = true;
    // TODO: wait handling == 0
}
