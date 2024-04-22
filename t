#!/bin/sh

#set -e

env EXP_NAME="local" faabric_dist_tests "Bench MPI all reduce local"

env EXP_NAME="remote" faabric_dist_tests "Bench MPI all reduce remote"

# env EXP_NAME="local" PAYLOAD="small" faabric_dist_tests "Bench MPI all reduce local"
# env EXP_NAME="local" PAYLOAD="large" faabric_dist_tests "Bench MPI all reduce local"
# env EXP_NAME="remote" PAYLOAD="small" faabric_dist_tests "Bench MPI all reduce remote"
# env EXP_NAME="remote" PAYLOAD="large" faabric_dist_tests "Bench MPI all reduce remote"

# env EXP_NAME="remote" faabric_dist_tests "Bench MPI all reduce remote"
