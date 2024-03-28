#pragma once

#include <memory>

namespace faabric::transport::tcp {
struct ConnectionFileDescriptorDeleter
{
    void operator()(int*);
};

class ConnectionFileDescriptor
{
  public:
    ConnectionFileDescriptor(int connFdIn);

    operator int();

  private:
    std::unique_ptr<int, ConnectionFileDescriptorDeleter> connFd;
};
}
