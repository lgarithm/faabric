#!/bin/sh

#set -e

export EXP_NAME="local"
faabric_dist_tests "Bench MPI all reduce local"

export EXP_NAME="remote"
faabric_dist_tests "Bench MPI all reduce remote"

