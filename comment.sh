#!/bin/sh

down() {
    local commit=$1
    echo "commit: $commit"
    local d=bench/$commit/faabric
    mkdir -p logs/$commit
    scp ko3:$d/1.txt "logs/$commit/1.txt"
    scp ko3:$d/2.txt "logs/$commit/2.txt"
}

cmt() {
    gh pr comment 2 --body "$1" --repo https://github.com/lgarithm/faabric
}

body() {
    local commit=$1
    echo "$commit"

    echo
    echo '```'
    cat "logs/$commit/1.txt"
    echo '```'
    echo

    echo '```'
    cat "logs/$commit/2.txt"
    echo '```'
}

f() {
    local commit=$1
    echo $commit
    down $commit
    cmt "$(body $commit)"
}

main() {
    local ids=$1
    for id in $(cat $ids | awk '{print $1}' | grep -v '#'); do
        echo "commit: $id"
        f $id
        echo "done for $id"
        echo
    done
}

main ids.txt
# cmt "$(body "7483943ede55cb90394eedaa670ec169239eeb0c.log")"
# cmt "$(body "8d10fa2051ff19cb141515296874d7d2bf2f3235.log")"
