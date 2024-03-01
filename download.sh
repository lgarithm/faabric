#!/bin/sh

down() {
    local b=$1
    echo "commit: $b"
    d=bench/$b/faabric
    scp ko3:$d/result.log "$b.log"
}

# down "7483943ede55cb90394eedaa670ec169239eeb0c"
down "8d10fa2051ff19cb141515296874d7d2bf2f3235"
