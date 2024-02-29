#!/bin/sh

bb() {
    local b=$1
    echo "commit: $b"
    d=$HOME/bench/$b/faabric
    if [ ! -d "$d" ]; then
        git clone git@github.com:/faasm/faabric.git $d
    fi
    cd $d
    if [ $(git remote | grep lg) ]; then
        echo "using exists lg"
    else
        git remote add lg git@github.com:lgarithm/faabric.git
    fi
    pwd
    git checkout -f $b
    git fetch lg
    git merge lg/bench-base --squash
    echo "merged"
    # ./bin/cli.sh
}

bb "98947e7212c60ad2e9124377eac4c25d9b825770"
