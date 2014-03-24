#!/bin/bash

rm -f runs.txt
touch runs.txt
for i in `seq 1 1000`; do
    ../bin/wallin >> runs.txt
done

rm -f runtimes.txt
touch runtimes.txt
grep Elapsed runs.txt >> runtimes.txt

./analyse_runtimes.rb runtimes.txt
