#!/usr/bin/env bash

set -eu

cc -g -O2 threads-memory.c -o threads-memory -lpthread

for ((n=100; n <= 1000; n += 50)); do
    ./threads-memory $n \
    | awk "/^total/ { print \"$n\\t\" \$3 \"\\t\" \$4 }"
done
