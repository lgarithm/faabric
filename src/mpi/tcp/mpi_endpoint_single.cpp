#include <cstring> // strerror

#include <functional> // function
#include <map>        // map
#include <memory>     // unique_ptr
#include <stdexcept>  // runtime_error
#include <thread>     // this_thread, thread
#include <utility>    // move

// #include <dist.fmt.hpp>
// #include <fmt_macros.hpp> // FMT
#include <log.hpp>
#include <mpi_endpoint.hpp>
#include <patient.hpp>
#include <socket_opt.h>
#include <sys/socket.h> // recv

// #define F(T, x) S(~FMT(T, x))

static void sleep_ms(int n)
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(n * 1ms);
}

struct mailbox
{
    file_descriptor fd;
};

using task = std::function<void()>;

void par(task f, task g)
{
    SLOW_SCOPE(__func__, 1);
    std::thread t(f);
    g();
    t.join();
}

struct mpi_endpoint_single : mpi_endpoint
{
    client_pool clis;
    std::unique_ptr<tcp_server> srv;

    using post_office = std::map<uint64_t, mailbox>;
    post_office po;

    void wait_others();

  public:
    void init() override;
    void fini() override;

    void send_exact(send_ptr, int, peer_id) override;
    void recv_exact(recv_ptr, int, peer_id) override;

    static mpi_endpoint_single& get()
    {
        static mpi_endpoint_single self;
        return self;
    }
};

#if USING_EP == EP_ST
DEF_MPI_ENDPOINT(mpi_endpoint_single);
#endif

void init_thread_id(const char* p = nullptr);

static bool blocking_recv(int fd, char* p, int n)
{
    SLOW_SCOPE(__func__, 1);
    int done = 0;
    for (; done < n;) {
        int got = recv(fd, p, n - done, 0);
        if (got < 0) {
            if (!would_block()) {
                PERR("%s calls recv(%d, ...): ", __func__, fd);
                throw std::runtime_error(__func__);
                return false;
            } else {
                ERR("not expected: %s", std::strerror(errno));
            }
        } else {
            p += got;
            done += got;
        }
    }
    return done == n;
}

void mpi_endpoint_single::wait_others()
{
    SLOW_SCOPE(__func__, 1);
    int fd = srv->start_listening();
    for (size_t others = me.peers.size() - 1; po.size() < others;) {
        if (int conn = accept(fd, 0, 0); conn > 0) {
            set_blocking(conn);
            uint64_t k = 0;
            if (!::blocking_recv(conn, (char*)&k, sizeof(k))) {
                ERR("hand shake failed: #%d", conn);
                continue;
            }
            if (po.count(k) > 0) {
                peer_id src = from_key(k);
                ERR("dup conn from %s", "? F(peer_id, src)");
            } else {
                mailbox mb = {
                    .fd = conn,
                };
                po.emplace(k, std::move(mb));
            }
        } else {
            if (!would_block()) {
                PERR("accept(%d, ...): ", fd);
            } else {
                sleep_ms(10);
            }
        }
    }
}

void mpi_endpoint_single::init()
{
    SLOW_SCOPE(__func__, 1);
    init_thread_id("main");

    auto [host, port] = me.peers[me.rank];
    srv.reset(new tcp_server(port, host));

    par([&]() { wait_others(); },
        [&]() {
            init_client_pool(*this, clis);
            mpi_hand_shake(*this, clis);
        });
}

void mpi_endpoint_single::fini()
{
    SLOW_SCOPE(__func__, 1);
    clis.clear();
    srv->stop();
}

void mpi_endpoint_single::send_exact(send_ptr x, int n, peer_id target)
{
    LOG_SCOPE_F("%s(%d)", __func__, n);
    tcp_client& cli = clis.at(key(target));
    cli.send_exact(x, n);
}

void mpi_endpoint_single::recv_exact(recv_ptr y, int n, peer_id target)
{
    SLOW_SCOPE(__func__, 1);
    auto& mb = po.at(key(target));
    blocking_recv(mb.fd, (char*)y, n);
}
