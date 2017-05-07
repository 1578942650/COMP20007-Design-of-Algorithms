#!/bin/bash

if [ -z $1 ]; then
    echo "./test.sh (folder with samples)"
    exit 0
fi

if ! [ -d $1 ]; then
    echo "Samples folder does not exists"
    exit 128
fi

make clean && make

if ! [ -d diffresult ]; then
    mkdir diffresult
fi

for n in {1..5}; do
    for i in {0..19}; do
        for j in {0..19}; do
            ./a1 -p $n -s $i -d $j -f vicroads.txt &> output
            diff --ignore-all-space output $1/output-p$n-s$i-d$j > diffresult/diff-p$n-s$i-d$j
            if ! [ -s diffresult/diff-p$n-s$i-d$j ]; then
                rm diffresult/diff-p$n-s$i-d$j
            fi
        done
    done
done

rm output
