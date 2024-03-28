#include <atomic>  // atomic
#include <thread>  // this_thread
#include <utility> // move

#include <mpi_endpoint.hpp>
#include <patient.hpp>

peer_id from_key(uint64_t k)
{
    uint32_t host = k >> 32;
    uint16_t port = k;
    return peer_id(host, port);
}

uint64_t key(uint32_t host, uint16_t port)
{
    uint64_t x = host;
    x <<= 32;
    x |= port;
    return x;
}

uint64_t key(peer_id p)
{
    auto [host, port] = p;
    return key(host, port);
}

int dial_all(client_pool &clis)
{
    int failed = 0;
    for (auto &[_, c] : clis) {
        failed += !c.dial();
    }
    return failed;
}

static void sleep_ms(int n)
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(n * 1ms);
}

void init_client_pool(const mpi_endpoint &ep, client_pool &clis)
{
    clis.clear();
    const int np = ep.me.peers.size();
    for (int i = 0; i < np; ++i) {
        if (i != ep.me.rank) {
            auto [host, port] = ep.me.peers[i];
            tcp_client cli(port, host);
            clis.emplace(key(host, port), std::move(cli));
        }
    }
    {
        WITH_PATIENT("dial all", 1);
        for (;;) {
            int failed = dial_all(clis);
            if (failed == 0) {
                break;
            }
            YIELD_PATIENT_("%d/%d failed", failed, (int)clis.size());
            sleep_ms(10);
        }
    }
}

void mpi_hand_shake(const mpi_endpoint &ep, client_pool &clis)
{
    uint64_t k = key(ep.me.peers[ep.me.rank]);
    for (auto &[_, cli] : clis) {
        cli.send_exact(&k, sizeof(k));
    }
}

void mpi_wait_others(const mpi_endpoint &ep, const std::atomic<int> &connected)
{
    int other = ep.me.peers.size() - 1;
    WITH_PATIENT("wait other peers");
    for (; connected < other;) {
        sleep_ms(50);
        YIELD_PATIENT_("only %d/%d had connected", (int)connected, other);
    }
}
