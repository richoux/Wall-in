#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 time_limit attempts" >&2
  exit 1
fi

for i in `seq 1 100`; do
    echo \*\*\*\*\*\*\*\*\*\*\*\* | cat >> scripts/xp_12th_of_May/unopt_new_100_runs_$1_$2
    echo \*\*\*\* $i \*\*\*\* | cat >> scripts/xp_12th_of_May/unopt_new_100_runs_$1_$2
    echo \*\*\*\*\*\*\*\*\*\*\*\* | cat >> scripts/xp_12th_of_May/unopt_new_100_runs_$1_$2
    ./run-experiments.sh $1 $2 >> scripts/xp_12th_of_May/unopt_new_100_runs_$1_$2
done