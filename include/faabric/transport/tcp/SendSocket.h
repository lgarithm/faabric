#pragma once

#include <faabric/transport/tcp/Address.h>
#include <faabric/transport/tcp/Socket.h>

namespace faabric::transport::tcp {
class SendSocket
{
  public:
    SendSocket(const std::string& host, int port);

    void dial();

    void sendOne(const uint8_t* buffer, size_t bufferSize);

    void sendMany(std::vector<uint8_t*> bufferVec,
                  std::vector<size_t> bufferSizeVec);

  private:
    Address addr;
    Socket sock;
    bool connected;

    std::string host;
    int port;
};
}
