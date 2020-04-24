#!/usr/bin/env bash

set -eu

cc -g -O2 threads-memory.c -o threads-memory -lpthread

echo -e "num threads\tvirtual KiB\tresident KiB"
for ((n=100; n <= 1000; n += 50)); do
    ./threads-memory $n \
    | awk -v num_threads=$n '/^total/ { print num_threads "\t" $3 "\t" $4 }'
    # | awk -v num_threads=$n '
    #     /Active . Total Size/ { print num_threads "\t" $8 }
    # '
done
