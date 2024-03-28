#pragma once

#include <dist.api.hpp>
#include <dist.types.hpp>
#include <mpi_endpoint.hpp>
#include <print_macros.hpp>

void run_graph_pair(const peer &me, const session &sess, mpi_workspace w,
                    ar_graph_pair gp);
