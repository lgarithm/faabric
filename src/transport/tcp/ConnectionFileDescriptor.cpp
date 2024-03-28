#include <faabric/transport/tcp/ConnectionFileDescriptor.h>

#include <unistd.h>

namespace faabric::transport::tcp {
ConnectionFileDescriptor::ConnectionFileDescriptor(int connFdIn)
{
    // TODO: why?
    int* p = new int;
    *p = connFdIn;
    connFd.reset(p);
    // connFd = std::make_unique<int,
    // ConnectionFileDescriptorDeleter>(connFdIn);
}

ConnectionFileDescriptor::operator int()
{
    return *connFd;
}

void ConnectionFileDescriptorDeleter::operator()(int* connFd)
{
    ::close(*connFd);
    delete connFd;
}
}
