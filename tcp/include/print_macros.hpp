#pragma once
#include <cstdio>  // fprintf, snprintf
#include <cstring> // strerror

#define PRN(fmt, ...) std::fprintf(stderr, fmt "\n", __VA_ARGS__)

#define ERR(fmt, ...) std::fprintf(stderr, fmt "\n", __VA_ARGS__)

#define S(x) (x).c_str()

#define PRN_IF(cond, fmt, ...)                                                 \
    if (cond) {                                                                \
        PRN(fmt, __VA_ARGS__);                                                 \
    }

#define SPRINTF(s, fmt, ...) std::snprintf(s, sizeof(s), fmt, __VA_ARGS__)

#define PERR(fmt, ...)                                                         \
    ERR(fmt "%s (errno=%d)", __VA_ARGS__, std::strerror(errno), errno)

#define PERR_IF(cond, fmt, ...)                                                \
    if (cond) {                                                                \
        PERR(fmt, __VA_ARGS__);                                                \
    }
