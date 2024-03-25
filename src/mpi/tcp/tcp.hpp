#pragma once
#include <atomic>     // atomic
#include <functional> // function
#include <memory>     // unique_ptr

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

class tcp_socket
{
    std::atomic<bool>* stopped;
    const int fd;

  public:
    tcp_socket(std::atomic<bool>* stopped = nullptr);

    tcp_socket(int fd, std::atomic<bool>* stopped = nullptr);

    virtual ~tcp_socket();

    int get();
};

using tcp_handler = std::function<void(std::unique_ptr<tcp_socket>)>;

class tcp_server
{
    tcp_addr addr;

    std::atomic<bool> stopped;
    std::atomic<int> handling;

    tcp_socket sock;

  public:
    tcp_server(int port, int host = 0);

    // virtual ~tcp_server();

    void start();

    void serve(tcp_handler);

    void stop();
};

class tcp_client
{
    tcp_addr addr;
    tcp_socket sock;

  public:
    tcp_client(int port, int host);

    void dial();

    void send_exact(const char*, int n);
};

class tcp_endpoint
{
  public:
};
