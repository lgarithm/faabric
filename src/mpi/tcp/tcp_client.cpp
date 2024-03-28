#include <arpa/inet.h> // sockaddr_in
#include <log.hpp>
#include <patient.hpp>
#include <print_macros.hpp>
#include <socket_opt.h>
#include <sys/socket.h> // send
#include <tcp.hpp>
#include <unistd.h> // close

tcp_client::tcp_client(int port, int host)
    : addr(port, host), sock(nullptr), connected(false), port(port), host(host)
{
}

bool tcp_client::dial()
{
    SLOW_SCOPE(__func__, 1);
    if (connected) {
        return true;
    }
    int fd = sock.get();
    if (int ret = connect(fd, addr.get(), sizeof(sockaddr_in)); ret) {
        PERR_IF(errno != ECONNREFUSED, "connect(%d, ...): ", fd);

        // create a new socket, or get 'Invalid argument' (EINVAL)
        // close(fd);
        sock = tcp_socket();
        // close(fd); // can't be reuse after close
        return false;
    }
    // LOG("connect(%d, ...): OK", fd);
    connected = true;
    return true;
}

void tcp_client::send_exact(const void *p, int n)
{
    if (int sent = send(sock.get(), p, n, 0); sent != n) {
        ERR("sent %d/%d", sent, n);
    }
}

void tcp_client::send_2_buffers(const char *head, const char *body,
                                int body_size)
{
    // call send_v(..)
}
