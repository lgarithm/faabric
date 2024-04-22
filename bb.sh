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

setup() {
    cli_inv apt update
    cli_inv apt update apt install -y linux-tools-common linux-tools-generic linux-tools-5.15.0-91-generic
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
    git merge lg/$bench_base --squash --strategy-option ours
    echo "merged"
    git status
    echo "building ..."
    setup
    run
    echo "done"
}

all() {
    local ids=$1
    for id in $(cat $ids | awk '{print $1}' | grep -v '#'); do
        echo "commit: $id"
        bb "$id"
        echo "done for $id"
    done
}

# inv docker.build -c openmpi-worker
# bb "7483943ede55cb90394eedaa670ec169239eeb0c" # main
all ids.txt

# bb "578c079e9a24d838485544a7039845f18864a08a" # merge failed
