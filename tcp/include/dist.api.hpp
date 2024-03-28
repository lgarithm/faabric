#pragma once
// clang-format off

#include <dist.types.hpp>

// barrier
void barrier(const peer & x_0, const session & x_1);

// all_reduce
void all_reduce(const peer & x_0, const session & x_1, send_ptr x_2, recv_ptr x_3, int x_4, type_name x_5, op_name x_6);

// new_graph
hw_topo_graph new_graph(int x_0);

// graph_add_edge
void graph_add_edge(hw_topo_graph & x_0, int x_1, int x_2);

// reversed_graph
hw_topo_graph reversed_graph(const hw_topo_graph & x_0);

// peer_start
void peer_start(peer x_0);

// join_session
session join_session(peer x_0);

// peer_stop
void peer_stop(peer x_0);

// set_loops
void set_loops(hw_topo_graph & x_0);

// vector_reduce
void vector_reduce(send_ptr x_0, send_ptr x_1, recv_ptr x_2, int x_3, type_name x_4, op_name x_5);
