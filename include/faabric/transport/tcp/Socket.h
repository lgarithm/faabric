#pragma once

#include <faabric/transport/tcp/ConnectionFileDescriptor.h>

#include <arpa/inet.h>
#include <atomic>
#include <functional>
#include <memory>
#include <sys/socket.h>

namespace faabric::transport::tcp {
class Socket
{
  public:
    Socket();
    Socket(int connFd);

    int get();

  private:
    ConnectionFileDescriptor connFd;
};

typedef std::function<void(std::unique_ptr<Socket>)> Handler;
typedef std::function<void(std::atomic<bool>&, int connFd)> PollingHandler;
}
