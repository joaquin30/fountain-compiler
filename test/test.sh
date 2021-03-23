#!/bin/sh

for file in test/*.fountain
do
    name=$(basename "$file" .fountain)
    mkdir -p test/src
    ./build/fountain "$file" -o "test/src/$name.html"
    ./build/fountain "$file" -o "test/src/$name.pdf" -f pdf
    ./build/fountain "$file" -o "test/src/$name.fdx" -f fdx
done
