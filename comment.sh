#!/bin/sh

cmt() {
    gh pr comment 2 --body "$1" --repo https://github.com/lgarithm/faabric
}

body() {
    echo "$1"
    echo
    echo '```'
    cat "$1"
    echo '```'
}

# cmt "$(body "7483943ede55cb90394eedaa670ec169239eeb0c.log")"
cmt "$(body "8d10fa2051ff19cb141515296874d7d2bf2f3235.log")"
