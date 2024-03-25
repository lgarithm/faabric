#include <cstdio>  // snprintf
#include <cstring> // memset

#include "print_macros.hpp"
#include "tcp.hpp"
#include <arpa/inet.h>  // inet_pton, sockaddr_in
#include <sys/socket.h> // sockaddr

static void unpack(uint32_t n, int& a, int& b, int& c, int& d)
{
    a = n >> 24;
    b = (n >> 16) & 0xff;
    c = (n >> 8) & 0xff;
    d = n & 0xff;
}

tcp_addr::tcp_addr(int port, int host)
{
    std::memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (host) {
        char ip[16];
        int a, b, c, d;
        unpack(host, a, b, c, d);
        std::snprintf(ip, sizeof(ip), "%d.%d.%d.%d", a, b, c, d);
        inet_pton(AF_INET, ip, &addr.sin_addr);
    }
}

sockaddr* tcp_addr::get() const
{
    return (sockaddr*)&addr;
}

void show_addr(sockaddr_in a)
{
    int port = ntohs(a.sin_port);
    PRN("port: %d", port);
    char ip[16];
    inet_ntop(AF_INET, &a.sin_addr, ip, sizeof(ip));
    PRN("ip: %s", ip);
}
