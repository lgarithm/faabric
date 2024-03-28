#include <stdio.h>

#include <fcntl.h> // F_GETFL, fcntl
#include <socket_opt.h>
#include <sys/socket.h> // setsockopt

void reuse_addr(int fd)
{
    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }
}

void set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl(fd, F_GETFL, 0) failed");
    }
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void set_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl(fd, F_GETFL, 0) failed");
    }
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
}

int is_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl(fd, F_GETFL, 0) failed");
    }
    return (flags & O_NONBLOCK) != 0;
}
