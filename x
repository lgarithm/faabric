#!/bin/sh
set -e

now_sec() {
    date +%s
}

measure() {
    echo "[->] $@ begins"
    local begin=$(now_sec)
    "$@"
    local end=$(now_sec)
    local duration=$((end - begin))
    echo "[==] $@ took ${duration}s" | tee -a time.log
}

main() {
    scp ids.txt bb.sh ko3:
    ssh ko3 ./bb.sh

    ./comment.sh
    ./plot/plot.sh
}

measure main
