#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 time_limit attempts" >&2
  exit 1
fi

for i in `seq 1 100`; do
    echo \*\*\*\*\*\*\*\*\*\*\*\* | cat >> scripts/100\ runs/100_runs_gap_$1_$2
    echo \*\*\*\* $i \*\*\*\* | cat >> scripts/100\ runs/100_runs_gap_$1_$2
    echo \*\*\*\*\*\*\*\*\*\*\*\* | cat >> scripts/100\ runs/100_runs_gap_$1_$2
    ./run-experiments.sh $1 $2 >> scripts/100\ runs/100_runs_gap_$1_$2
done