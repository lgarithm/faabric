#!/bin/sh
set -e

with_cd() {
    back=$PWD
    cd $1
    shift
    $@
    cd $back
}

cxx-fix ./src/mpi/tcp tcp/examples
with_cd tcp ./x
