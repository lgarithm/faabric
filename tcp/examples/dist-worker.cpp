#include <algorithm> // ranges
#include <ranges>    // ranges
#include <string>    // string
#include <vector>    // vector

#include <dist.hpp>
// #include <ldl_macros.hpp> // DECL_FILE_PARSER, PARSE_FILE
#include <log.hpp>
#include <print_macros.hpp>
// #include <unwrap.hpp> // unwrap

void disable_stdout_buffer()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
}

void g(const peer& me, const session& sess, int n)
{
    LOG_IF(me.rank == 0, "%s(%d) | np = %d", __func__, n, (int)me.peers.size());
    using T = int32_t;
    std::vector<T> x(n);
    std::vector<T> y(n);
    std::ranges::fill(x, 1);
    std::ranges::fill(y, 0);
    all_reduce(me, sess, x.data(), y.data(), n, "i32", "sum");
    LOG_IF(me.rank == 0, "y[0]=%d", y[0]);
}

// DECL_FILE_PARSER(int_list_file, _);

void f(const peer& me, const session& sess, std::string filename)
{
    // auto x = PARSE_FILE(int_list_file, filename);
    // if (!x) {
    //     return;
    // }

    // for (auto n : unwrap(x.value())) {
    //     g(me, sess, n);
    // }
}

int main(int argc, char* argv[])
{
    disable_stdout_buffer();
    auto& ep = mpi_endpoint::get();
    ep.init();

    if (argc == 2) {
        barrier(ep.me, ep.sess);
        f(ep.me, ep.sess, argv[1]);
    }

    ep.fini();
    return 0;
}
