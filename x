#!/bin/sh
set -e

scp ids.txt bb.sh ko3:
ssh ko3 ./bb.sh

# ./plot/plot.sh
