#pragma once
#include <cstdio> // fprintf

#include <string> // string

extern thread_local char tid[32];

#define LOG(fmt, ...) std::fprintf(stderr, "[%s] " fmt "\n", tid, __VA_ARGS__)

#define LOG_IF(cond, fmt, ...)                                                 \
    if (cond) {                                                                \
        LOG(fmt, __VA_ARGS__);                                                 \
    }

struct scope_logger
{
    std::string name;

    scope_logger(std::string name);
    ~scope_logger();
};

// #define LOG_SCOPE(name) scope_logger __scope_logger(name);
#define LOG_SCOPE(_)

#define LOG_SCOPE_F(fmt, ...)                                                  \
    thread_local char __log_name[1 << 8];                                      \
    SPRINTF(__log_name, fmt, __VA_ARGS__);                                     \
    LOG_SCOPE(__log_name);
