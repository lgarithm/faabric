#pragma once
#include <cstdio> // fprintf

#define PRN(fmt, ...) std::fprintf(stdout, fmt "\n", __VA_ARGS__)
#define ERR(fmt, ...) std::fprintf(stderr, fmt "\n", __VA_ARGS__)
#define S(x) (x).c_str()

#define PRN_IF(cond, fmt, ...)                                                 \
    if (cond) {                                                                \
        PRN(fmt, __VA_ARGS__);                                                 \
    }
