#include "tcp.hpp"
#include <cstdio>  // snprintf
#include <cstring> // strcmp

void run_srv(int port, int host = 0)
{
    tcp_server s(port, host);
    s.start();
}

void run_cli(int port, int host = 0)
{
    const int Mi = 1 << 20;
    char data[1 * Mi];
    tcp_client cli(port, host);
    cli.dial();
    int size = std::snprintf(data, sizeof(data), "%s", "hello world");
    cli.send_exact(data, size);
}

void usage() {}

void run_role(const char* role)
{
    int port = 18080;
    if (std::strcmp(role, "s") == 0) {
        run_srv(port);
    } else if (std::strcmp(role, "c") == 0) {
        run_cli(port);
    } else {
        usage();
    }
}

int tcp_endpoint_main(int argc, char* argv[])
{
    if (argc <= 1) {
        usage();
        return 0;
    }
    run_role(argv[1]);
    return 0;
}
