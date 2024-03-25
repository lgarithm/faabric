#include <cstdio> // perror

#include "print_macros.hpp"
#include "tcp.hpp"
#include <arpa/inet.h>  // sockaddr_in
#include <sys/socket.h> // send, sockaddr
#include <unistd.h>     // close

tcp_client::tcp_client(int port, int host)
  : addr(port, host)
  , sock(nullptr)
{}

void tcp_client ::dial()
{
    if (int ret = connect(sock.get(), (sockaddr*)&addr, sizeof(sockaddr_in));
        ret) {
        std::perror("connect failed");
    }
}

void tcp_client::send_exact(const char* p, int n)
{
    if (int sent = send(sock.get(), p, n, 0); sent != n) {
        ERR("sent %d/%d", sent, n);
    }
}
