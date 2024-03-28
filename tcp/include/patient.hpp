#pragma once
#include <cstdint>
#include <cstdio> // snprintf

#include <atomic>  // atomic
#include <chrono>  // chrono
#include <memory>  // unique_ptr
#include <string>  // string
#include <thread>  // thread
#include <utility> // move

#include <print_macros.hpp>
#include <stdtracer_xterm.hpp>

class patient_t
{
    using Clock = std::chrono::high_resolution_clock;
    using Instant = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<double>;

    const std::string name;
    const Instant t0_;
    const Duration threshold_;
    Instant last_;
    bool once_failed;

    FILE *fp_;
    bool report_;

  public:
    patient_t(std::string name = "");

    patient_t(std::string name, int n)
        : patient_t(std::chrono::seconds(n), std::move(name))
    {
    }

    patient_t(std::string name, Duration threshold)
        : patient_t(threshold, std::move(name))
    {
    }

    patient_t(Duration threshold = std::chrono::seconds(1),
              std::string name = "", FILE *fp = stderr)
        : name(std::move(name)), t0_(Clock::now()), threshold_(threshold),
          last_(t0_), once_failed(false), fp_(fp), report_(true)
    {
    }

    ~patient_t();

    void report(std::string msg) const;

    void reset()
    {
        once_failed |= !ok();
        last_ = Clock::now();
    }

    bool ok() const
    {
        return (Clock::now() - last_) < threshold_;
    }
};

#define WITH_PATIENT(...) patient_t __patient(__VA_ARGS__);

#define YIELD_PATIENT(msg)                                                     \
    {                                                                          \
        if (!__patient.ok()) {                                                 \
            __patient.report(msg);                                             \
            __patient.reset();                                                 \
        }                                                                      \
    }

#define SPRINTF(s, fmt, ...) std::snprintf(s, sizeof(s), fmt, __VA_ARGS__)

#define YIELD_PATIENT_(fmt, ...)                                               \
    {                                                                          \
        thread_local char msg[128];                                            \
        SPRINTF(msg, fmt, __VA_ARGS__);                                        \
        YIELD_PATIENT(msg)                                                     \
    }

struct slow_scope {
    using Clock = std::chrono::high_resolution_clock;
    using Instant = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<double>;

    std::string name;
    const Instant t0;

    std::atomic<bool> done;
    std::unique_ptr<std::thread> poller;

    slow_scope(std::string name, int n);

    ~slow_scope();
};

#define SLOW_SCOPE(name, x)

// #define SLOW_SCOPE(name, x) slow_scope __slow_scope(name, x);
