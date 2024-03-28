#pragma once
#include <atomic>        // atomic
#include <unordered_map> // unordered_map

#include <dist.api.hpp>
#include <dist.types.hpp>

peer init_dist();

struct mpi_endpoint
{
    peer me;
    session sess;

    virtual void init() = 0;
    virtual void fini() = 0;

    virtual void send_exact(send_ptr, int, peer_id) = 0;
    virtual void recv_exact(recv_ptr, int, peer_id) = 0;

    static mpi_endpoint& get();

    mpi_endpoint()
      : me(init_dist())
      , sess(join_session(me))
    {}
};

#define EP_THREAD 1
#define EP_POLL 2
#define EP_ST 3
#define EP_CORO 4

// #define USING_EP EP_THREAD
// #define USING_EP EP_POLL
#define USING_EP EP_ST
// #define USING_EP EP_CORO

#define DEF_MPI_ENDPOINT(T)                                                    \
    mpi_endpoint& mpi_endpoint::get()                                          \
    {                                                                          \
        return T::get();                                                       \
    }

#include <tcp.hpp>

using client_pool = std::unordered_map<uint64_t, tcp_client>;

void init_client_pool(const mpi_endpoint& ep, client_pool& clis);

void mpi_hand_shake(const mpi_endpoint& ep, client_pool& clis);
void mpi_wait_others(const mpi_endpoint& ep, const std::atomic<int>& connected);

uint64_t key(peer_id p);

peer_id from_key(uint64_t k);
