#!/bin/sh

bench_base=bb

cli_inv() { docker compose exec -it cli ./bin/inv_wrapper.sh $@; }

bench_named() {
    docker compose exec -it cli env EXP_NAME="$1" ./build/static/bin/faabric_dist_tests "$2"
}

bench() {
    bench_named "local" "Bench MPI all reduce local" | grep bench_allreduce | tee 1.txt
    bench_named "remote" "Bench MPI all reduce remote" | grep bench_allreduce | tee 2.txt
}

run() {
    docker compose down
    docker compose up --no-recreate -d cli
    FAABRIC_DOCKER="on" ./bin/wait_for_venv.sh

    cli_inv dev.cmake -b Release

    cli_inv dev.cc faabric_dist_tests
    cli_inv dev.cc faabric_dist_test_server
    cli_inv dev.cc planner_server
    ./dist-test/dev_server.sh

    bench

    # docker compose down
}

bb() {
    local commit=$1
    echo "commit: $commit"
    local d=$HOME/bench/$commit/faabric
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
    git checkout -f $commit
    git fetch lg
    git merge lg/$bench_base --squash
    echo "merged"
    git status
    echo "building ..."
    if [ -f result.log ]; then
        echo "result exists"
    fi

    run
    # ./bin/cli.sh

    echo "done"
}

all() {
    for id in $(cat ids.txt | awk '{print $1}'); do
        echo "commit: $id"
        bb "$id"
        echo "done for $id"
    done
}

# bb "7483943ede55cb90394eedaa670ec169239eeb0c" # main
all
