#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void reuse_addr(int fd);

void set_non_blocking(int fd);
void set_blocking(int fd);

int is_non_blocking(int fd);

#ifdef __cplusplus
}
#endif
