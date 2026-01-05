#!/bin/bash

if [ -z "$COOKIE" ]; then
    echo "Error: COOKIE is not set" 1>&2
    exit 1
fi

for i in {1..10}
do
    printf -v padded "%02d" "$i"

    curl -b "session=$COOKIE" \
        "https://adventofcode.com/2025/day/$i/input" > inputs/$padded.txt
done
