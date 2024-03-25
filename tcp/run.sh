#!/bin/sh
set -e

./bin/tcp-endpoint s &
PID=$!

echo "server: #<$PID>"

./bin/tcp-endpoint c
