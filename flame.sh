#!/bin/sh

#
# apt update
# apt install -y linux-tools-common linux-tools-generic linux-tools-$(uname -r)

perf record -F 99 -p $(pidof faabric_dist_tests) -g -- sleep 60 # -> perf.data

rm -fr out.perf
rm -fr out.folded
rm -fr kernel.svg

perf script -f >out.perf
./FlameGraph/stackcollapse-perf.pl out.perf >out.folded
./FlameGraph/flamegraph.pl out.folded >kernel.svg
