#include <faabric/transport/tcp/SocketOptions.h>
#include <faabric/util/logging.h>

#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

namespace faabric::transport::tcp {
void setReuseAddr(int connFd)
{
    // TODO: do we need to check the value populated in opt?
    int opt = 1;
    int ret = setsockopt(connFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret < 0) {
        SPDLOG_ERROR("setsockopt(SO_REUSEADDR) failed on socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting socket option: SO_REUSADDR");
    }
}

void setNoDelay(int connFd)
{
    int opt = 1;
    int ret = setsockopt(connFd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    if (ret < 0) {
        SPDLOG_ERROR("setsockopt(TCP_NODELAY) failed on socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting socket option: TCP_NODELAY");
    }
}

void setQuickAck(int connFd)
{
    int opt = 1;
    int ret = setsockopt(connFd, IPPROTO_TCP, TCP_QUICKACK, &opt, sizeof(opt));
    if (ret < 0) {
        SPDLOG_ERROR("setsockopt(TCP_QUICKACK) failed on socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting socket option: TCP_QUICKACK");
    }
}

void setNonBlocking(int connFd)
{
    int flags = fcntl(connFd, F_GETFL, 0);
    if (flags < 0) {
        SPDLOG_ERROR("fcntl(fd, F_GETFL, 0) failed on socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting socket as non-blocking");
    }

    // TODO: check return value?
    fcntl(connFd, F_SETFL, flags | O_NONBLOCK);
}

void setBlocking(int connFd)
{
    int flags = fcntl(connFd, F_GETFL, 0);
    if (flags < 0) {
        SPDLOG_ERROR("fcntl(fd, F_GETFL, 0) failed on socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting socket as blocking");
    }

    // TODO: check return value?
    fcntl(connFd, F_SETFL, flags & ~O_NONBLOCK);
}

bool isNonBlocking(int connFd)
{
    int flags = fcntl(connFd, F_GETFL, 0);
    if (flags < 0) {
        SPDLOG_ERROR("fcntl(fd, F_GETFL, 0) failed on socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error checking if socket is blocking");
    }

    return (flags & O_NONBLOCK) != 0;
}

// Set busy polling in kernel space
void setBusyPolling(int connFd)
{
    // Number of microseonds to busy poll in kernel space
    int numMicroSecToPoll = 10000;

    int ret = setsockopt(connFd,
                         SOL_SOCKET,
                         SO_BUSY_POLL,
                         &numMicroSecToPoll,
                         sizeof(numMicroSecToPoll));
    if (ret == -1) {
        SPDLOG_ERROR("Error setting kernel busy poll for socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting kernel busy poll");
    }
}

void setRecvTimeoutMs(int connFd, int timeoutMs)
{
    struct timeval timeVal;
    timeVal.tv_sec = timeoutMs / 1000;
    timeVal.tv_usec = (timeoutMs % 1000) * 1e3;

    int ret = ::setsockopt(
      connFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeVal, sizeof(timeVal));
    if (ret == -1) {
        SPDLOG_ERROR("Error setting recv timeout for socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting recv timeout");
    }
}

void setSendTimeoutMs(int connFd, int timeoutMs)
{
    struct timeval timeVal;
    timeVal.tv_sec = timeoutMs / 1000;
    timeVal.tv_usec = 0;

    int ret = ::setsockopt(
      connFd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeVal, sizeof(timeVal));
    if (ret == -1) {
        SPDLOG_ERROR("Error setting send timeout for socket {}: {}",
                     connFd,
                     std::strerror(errno));
        throw std::runtime_error("Error setting send timeout");
    }
}

void setRecvBufferSize(int connFd, size_t bufferSizeBytes)
{
    int ret = ::setsockopt(
      connFd, SOL_SOCKET, SO_RCVBUF, &bufferSizeBytes, sizeof(bufferSizeBytes));
    if (ret == -1) {
        SPDLOG_ERROR(
          "Error setting recv buffer size for socket {}: {} (no: {})",
          connFd,
          std::strerror(errno),
          errno);
        throw std::runtime_error("Error setting recv buffer size");
    }
}

void setSendBufferSize(int connFd, size_t bufferSizeBytes)
{
    int ret = ::setsockopt(
      connFd, SOL_SOCKET, SO_SNDBUF, &bufferSizeBytes, sizeof(bufferSizeBytes));
    if (ret == -1) {
        SPDLOG_ERROR(
          "Error setting send buffer size for socket {}: {} (no: {})",
          connFd,
          std::strerror(errno),
          errno);
        throw std::runtime_error("Error setting send buffer size");
    }
}
}
