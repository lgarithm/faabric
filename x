#!/bin/sh

set -e

# inv dev.cmake --build Release

inv dev.cc faabric_dist_tests
inv dev.cc faabric_dist_test_server
inv dev.cc planner_server