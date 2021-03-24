#!/bin/sh
mkdir -p test/output
for file in test/*.fountain
do
    name=$(basename "$file" .fountain)
    mkdir -p test/src
    ./build/fountain "$file" -o "test/output/$name.html"
    ./build/fountain "$file" -o "test/output/$name.pdf" -f pdf
    ./build/fountain "$file" -o "test/output/$name.fdx" -f fdx
done
