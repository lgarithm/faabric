#include <memory>    // make_unique
#include <stdexcept> // runtime_error
#include <thread>    // this_thread
#include <utility>   // move

#include <arpa/inet.h> // sockaddr_in
#include <log.hpp>
#include <print_macros.hpp>
#include <socket_opt.h>
#include <sys/socket.h> // bind, listen, sockaddr
#include <tcp.hpp>

bool would_block()
{
    return errno == EAGAIN;
}

tcp_server::tcp_server(int port, int host)
    : addr(port, host), stopped(false), handling(0), sock(&stopped)
{
}

void tcp_server::listen_loop(tcp_handler handle)
{
    LOG_SCOPE_F("BGN %s", __func__);
    int fd = sock.get();
    int nc = 0;
    for (; !stopped;) {
        sockaddr_in remote;
        socklen_t addr_len;
        if (int conn = accept(fd, (sockaddr *)&remote, &addr_len); conn > 0) {
            ++nc;
            if (false) {
                show_addr(remote, addr_len);
            }
            // n - 1 at most ideally
            LOG_IF(false,
                   "accept: %d, is_non_blocking: %d",
                   conn,
                   is_non_blocking(conn));
            set_non_blocking(conn);

            auto sock = std::make_unique<tcp_socket>(conn);
            // LOG("will handle: %d", sock->get());
            handle(std::move(sock));

        } else {
            if (!would_block()) {
                PERR("accept(%d, ...): ", fd);
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            // break;
        }
    }
}

void tcp_server::polling_loop(polling_handler handle)
{
    int fd = start_listening();
    handle(stopped, fd);
}

void tcp_server::serve(tcp_handler f)
{
    start_listening();
    listen_loop(f);
}

int tcp_server::start_listening()
{
    int fd = sock.get();
    reuse_addr(fd);
    if (!is_non_blocking(fd)) {
        // LOG("server: %d was not non_blocking", fd);
        set_non_blocking(fd);
    }
    if (int ret = bind(fd, addr.get(), sizeof(sockaddr_in)); ret) {
        PERR("bind(%d, ...): ", fd);
        if (errno == EADDRINUSE) { // linux: 98, osx: 48
            ERR("TODO: %s", "check port usage");
        }
        throw std::runtime_error(__func__);
        // return -1;
    }
    if (int ret = listen(fd, 1024); ret) {
        PERR("listen(%d, ...): ", fd);
        throw std::runtime_error(__func__);
        // return -1;
    }
    return fd;
}

void tcp_server::stop()
{
    stopped = true;
    // TODO: wait handling == 0
}
