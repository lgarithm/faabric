#!/bin/sh
set -e

list_ids() {
    local ids=$1
    echo mpi
    cat $ids | awk '{print $1}' | grep -v '#'
}

all() {
    ./plot/parse-log.py $(list_ids $1)
}

all ids.txt
