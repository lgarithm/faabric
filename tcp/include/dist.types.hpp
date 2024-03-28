#pragma once
// clang-format off

#include <memory>
#include <string>
#include <tuple>
#include <vector>

// #include <adt.hpp>

using peer_id = std::tuple<uint32_t, uint16_t>;

using peer_id_list = std::vector<peer_id>;

struct decimal_ip {
    int a;
    int b;
    int c;
    int d;
};

struct decimal_ip_port {
    decimal_ip ip;
    int port;
};

using decimal_ip_port_list = std::vector<decimal_ip_port>;

struct cluster {
    std::vector<peer_id> peers;
};

struct peer {
    int rank;
    std::vector<peer_id> peers;
};

using topo_node_id_list = std::vector<int>;

struct topo_node {
    int rank;
    bool self_loop;
    topo_node_id_list prevs;
    topo_node_id_list nexts;
};

struct hw_topo_graph {
    std::vector<topo_node> nodes;
};

struct ar_graph_pair {
    hw_topo_graph reduce;
    hw_topo_graph bcast;
};

using ar_graph_pair_list = std::vector<ar_graph_pair>;

struct session {
    std::vector<ar_graph_pair> topos;
};

using send_ptr = const void *;

using recv_ptr = void *;

using type_name = const char *;

using op_name = const char *;

struct mpi_workspace {
    send_ptr send_buf;
    recv_ptr recv_buf;
    int count;
    type_name type;
    op_name op;
};

struct int_list_file {
    std::vector<int> _;
};

// #include <type_macros.hpp>

// DEF_C(peer_id, std::tuple<uint32_t, uint16_t> x0);
// DEF_C(peer_id_list, std::vector<peer_id> x0);
// DEF_C(decimal_ip, int x0, int x1, int x2, int x3);
// DEF_C(decimal_ip_port, decimal_ip x0, int x1);
// DEF_C(decimal_ip_port_list, std::vector<decimal_ip_port> x0);
// DEF_C(cluster, std::vector<peer_id> x0);
// DEF_C(peer, int x0, std::vector<peer_id> x1);
// DEF_C(topo_node_id_list, std::vector<int> x0);
// DEF_C(topo_node, int x0, bool x1, topo_node_id_list x2, topo_node_id_list x3);
// DEF_C(hw_topo_graph, std::vector<topo_node> x0);
// DEF_C(ar_graph_pair, hw_topo_graph x0, hw_topo_graph x1);
// DEF_C(ar_graph_pair_list, std::vector<ar_graph_pair> x0);
// DEF_C(session, std::vector<ar_graph_pair> x0);
// DEF_C(send_ptr, const void * x0);
// DEF_C(recv_ptr, void * x0);
// DEF_C(type_name, const char * x0);
// DEF_C(op_name, const char * x0);
// DEF_C(mpi_workspace, send_ptr x0, recv_ptr x1, int x2, type_name x3, op_name x4);
// DEF_C(int_list_file, std::vector<int> x0);
