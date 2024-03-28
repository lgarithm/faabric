#pragma once
#include <atomic>     // atomic
#include <functional> // function
#include <memory>     // unique_ptr

#include <arpa/inet.h>  // sockaddr_in
#include <sys/socket.h> // sockaddr

bool would_block();

class tcp_addr
{
    sockaddr_in addr;

  public:
    tcp_addr(int port, int host = 0);

    sockaddr* get() const;
};

struct fd_deleter
{
    void operator()(int*);
};

class file_descriptor
{
    std::unique_ptr<int, fd_deleter> fd;

  public:
    file_descriptor(int fd);

    operator int();
};

class tcp_socket
{
    std::atomic<bool>* stopped;
    file_descriptor fd;

  public:
    tcp_socket(std::atomic<bool>* stopped = nullptr);

    tcp_socket(int fd, std::atomic<bool>* stopped = nullptr);

    int get();
};

using tcp_handler = std::function<void(std::unique_ptr<tcp_socket>)>;
using polling_handler = std::function<void(std::atomic<bool>&, int fd)>;

class tcp_server
{

    tcp_addr addr;

    std::atomic<bool> stopped;
    std::atomic<int> handling;

    tcp_socket sock;

    void listen_loop(tcp_handler);

  public:
    tcp_server(int port, int host = 0);

    void start();
    void stop();

    // only one of serve or polling_loop can be called
    void serve(tcp_handler);

    void polling_loop(polling_handler);

    int start_listening();
};

class tcp_client
{
    tcp_addr addr;
    tcp_socket sock;
    bool connected;

    void _send_exact(const char*, int n);

    int port;
    int host;

  public:
    // tcp_client(std::string host, int port);

    tcp_client(int port, int host);

    bool dial();

    void send_exact(const void*, int n);

    static constexpr int header_size = 32;
    // using header =

    void send_2_buffers(const char* head, const char* body, int body_size);
};

class tcp_endpoint
{
  public:
};

// void show_addr(sockaddr_in a);
void show_addr(sockaddr_in a, socklen_t addr_len);
