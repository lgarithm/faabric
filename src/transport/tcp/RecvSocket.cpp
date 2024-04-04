#include <faabric/transport/tcp/RecvSocket.h>
#include <faabric/transport/tcp/SocketOptions.h>
#include <faabric/util/logging.h>

namespace faabric::transport::tcp {
RecvSocket::RecvSocket(int port, const std::string& host)
  : addr(host, port)
{}

void RecvSocket::listen()
{
    int connFd = sock.get();
    reuseAddr(connFd);

    if (!isNonBlocking(connFd)) {
        // LOG("server: %d was not non_blocking", fd);
        setNonBlocking(connFd);
    }

    if (int ret = ::bind(connFd, addr.get(), sizeof(sockaddr_in)); ret) {
        SPDLOG_ERROR("TCP server error binding to fd {}: {} (ret: {})",
                     connFd,
                     std::strerror(errno),
                     ret);
        throw std::runtime_error("TCP Server error binding to fd");
    }

    if (int ret = ::listen(connFd, 1024); ret) {
        SPDLOG_ERROR("Error listening to socket fd {}: {} (ret: {})",
                     connFd,
                     std::strerror(errno),
                     ret);
        throw std::runtime_error("TCP Server error listening to fd");
    }

    // return connFd;
}

// TODO: make connFd part of the object
int RecvSocket::accept()
{
    int conn = ::accept(sock.get(), 0, 0);
    // setBlocking(conn);

    return conn;
}

void RecvSocket::recvOne(int conn, uint8_t* buffer, size_t bufferSize)
{
    size_t numRecvd = 0;

    while (numRecvd < bufferSize) {
        // Receive from socket
        int got = ::recv(conn, buffer, bufferSize - numRecvd, 0);
        if (got < 0) {
            // TODO: why?
            if (errno != EAGAIN) {
                SPDLOG_ERROR("TCP Server error receiving in {}: {}",
                             conn,
                             std::strerror(errno));
                throw std::runtime_error("TCP error receiving!");
            }

            // TODO: ??
            // SPDLOG_ERROR("not expected: {}", std::strerror(errno));
        } else {
            buffer += got;
            numRecvd += got;
        }
    }

    assert(numRecvd == bufferSize);
}
}
