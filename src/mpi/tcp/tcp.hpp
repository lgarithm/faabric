#pragma once
#include <atomic> // atomic

#include <arpa/inet.h>  // sockaddr_in
#include <sys/socket.h> // sockaddr

class tcp_addr
{
    sockaddr_in addr;

  public:
    tcp_addr(int port, int host = 0);

    // virtual ~tcp_addr();

    sockaddr* get() const;
};

class tcp_server
{
    tcp_addr addr;
    int fd;
    std::atomic<bool> stopped;

  public:
    tcp_server(int port, int host = 0);

    virtual ~tcp_server();

    void start();

    void stop();
};

class tcp_client
{
    tcp_addr addr;
    int fd;

  public:
    tcp_client(int port, int host);

    virtual ~tcp_client();

    void dial();

    void send_exact(const char*, int n);
};
