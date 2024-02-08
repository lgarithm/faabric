#include <faabric/mpi/mpi.h>
#include <faabric/util/logging.h>
#include <faabric/util/macros.h>

#include "mpi/mpi_native.h"

#include <chrono>
#include <thread>

using CLOCK = std::chrono::high_resolution_clock;
using duration = std::chrono::duration<double>;

constexpr int Ki = 1 << 20;
constexpr int Mi = 1 << 20;

std::string show_size(int workload)
{
    char buf[64];
    if (workload > Mi) {
        double unit = static_cast<double>(Mi);
        sprintf(buf, "%.3f%s", workload / unit, "MiB");
    } else {
        sprintf(buf, "%d%s", workload, "B");
    }
    return buf;
}

std::string show_rate(int workload, duration d)
{
    double unit = static_cast<double>(Mi);
    double r = static_cast<double>(workload) / unit / d.count();
    char buf[64];
    sprintf(buf, "%.3f%s/s", r, "MiB");
    return buf;
}

namespace tests::mpi {

static int bench_all_reduce(int rank, int worldSize, int n = 1 * Mi)
{
    using T = int;
    auto dt = MPI_INT;

    // int n = 1 * Mi;
    int workload = 4 * (worldSize - 1) * sizeof(T) * n;

    std::vector<T> x(n);
    std::vector<T> y(n);
    std::ranges::fill(x, rank);
    std::ranges::fill(y, 0);

    auto t0 = CLOCK::now();
    MPI_Allreduce(x.data(), y.data(), 3, dt, MPI_SUM, MPI_COMM_WORLD);
    auto t1 = CLOCK::now();
    duration d = t1 - t0;
    std::cerr << __func__ << " took " << d.count() << "workload: " << show_size(workload) << " rate: " << show_rate(workload, d) << std::endl;

    return 0;
}

int lg_benchmark()
{
    const std::string h40(40, '=');
    std::cerr << "BGN " << h40 << __func__ << h40 << std::endl;

    MPI_Init(NULL, NULL);

    int rank;
    int worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    SPDLOG_WARN("{}: {}/{}", __func__, rank, worldSize);

    std::vector<int> ns = {
        1 * Mi,
        2 * Mi,
        4 * Mi,
    };

    int ret = 0;
    for (int n : ns){
        ret = bench_all_reduce(rank, worldSize, n);
    }

    MPI_Finalize();

    std::cerr << "END " << h40 << __func__ << h40 << std::endl;

    return ret;
}
}
