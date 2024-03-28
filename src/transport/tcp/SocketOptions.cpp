#include <faabric/transport/tcp/SocketOptions.h>
#include <faabric/util/logging.h>

#include <fcntl.h>
#include <sys/socket.h>

namespace faabric::transport::tcp {
void reuseAddr(int connFd)
{
    int enable = 1;
    if (setsockopt(connFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
        0) {
        SPDLOG_ERROR("setsockopt(SO_REUSEADDR) failed on fd: {}", connFd);
        // TODO: trhow?
    }
}

void setNonBlocking(int connFd)
{
    int flags = fcntl(connFd, F_GETFL, 0);
    if (flags < 0) {
        SPDLOG_ERROR("fcntl(fd, F_GETFL, 0) failed on fd: {}", connFd);
        // TODO: trhow?
    }
    fcntl(connFd, F_SETFL, flags | O_NONBLOCK);
}

void setBlocking(int connFd)
{
    int flags = fcntl(connFd, F_GETFL, 0);
    if (flags < 0) {
        SPDLOG_ERROR("fcntl(fd, F_GETFL, 0) failed on fd: {}", connFd);
        // TODO: throw?
    }
    fcntl(connFd, F_SETFL, flags & ~O_NONBLOCK);
}

bool isNonBlocking(int connFd)
{
    int flags = fcntl(connFd, F_GETFL, 0);
    if (flags < 0) {
        SPDLOG_ERROR("fcntl(fd, F_GETFL, 0) failed on fd: {}", connFd);
        // TODO: throw?
    }
    return (flags & O_NONBLOCK) != 0;
}
}
