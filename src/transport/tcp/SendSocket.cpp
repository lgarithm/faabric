#include <faabric/transport/tcp/SendSocket.h>
#include <faabric/util/logging.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

namespace faabric::transport::tcp {
SendSocket::SendSocket(const std::string& host, int port)
  : addr(host, port)
  , host(host)
  , port(port)
{}

void SendSocket::dial()
{
    if (connected) {
        return;
    }

    int connFd = sock.get();
    if (int ret = connect(connFd, addr.get(), sizeof(sockaddr_in)); ret) {
        SPDLOG_ERROR("Error connecting to {}:{}: {} (ret: {})",
                     port,
                     host,
                     std::strerror(errno),
                     ret);
        throw std::runtime_error(
          "TCP SendSocket error connecting to remote address");
    }

    // TODO: maybe trace
    SPDLOG_DEBUG("TCP client connected to {}:{}", port, host);
    connected = true;
}

void SendSocket::sendOne(const uint8_t* buffer, size_t bufferSize)
{
    size_t sent = send(sock.get(), buffer, bufferSize, 0);
    if (sent != bufferSize) {
        SPDLOG_ERROR(
          "TCP client error sending TCP message to {}:{} ({}/{}): {}",
          host,
          port,
          sent,
          bufferSize,
          std::strerror(errno));
        throw std::runtime_error("TCP client error sending message!");
    }
}

void SendSocket::sendMany(std::vector<uint8_t*> bufferVec,
                          std::vector<size_t> bufferSizeVec)
{
    const int numMsgs = bufferVec.size();
    std::vector<struct iovec> msgs;

    size_t totalSize = 0;
    for (int i = 0; i < numMsgs; i++) {
        msgs.push_back(
          { .iov_base = bufferVec.at(i), .iov_len = bufferSizeVec.at(i) });
        totalSize += bufferSizeVec.at(i);
    }

    auto nSent = ::writev(sock.get(), msgs.data(), numMsgs);
    if (nSent != totalSize) {
        SPDLOG_ERROR("Error sending many to {}:{} ({}/{}): {}",
                     host,
                     port,
                     nSent,
                     totalSize,
                     std::strerror(errno));
        throw std::runtime_error("TCP SendSocket sendMany error!");
    }
}
}
