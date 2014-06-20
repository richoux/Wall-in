#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 time_limit attempts" >&2
  exit 1
fi

for i in `seq 1 100`; do
    echo \*\*\*\*\*\*\*\*\*\*\*\* | cat >> scripts/ghost/xp_20th_of_June/gap_with_opti_constraints_100_runs_$1_$2
    echo \*\*\*\* $i \*\*\*\* | cat >> scripts/ghost/xp_20th_of_June/gap_with_opti_constraints_100_runs_$1_$2
    echo \*\*\*\*\*\*\*\*\*\*\*\* | cat >> scripts/ghost/xp_20th_of_June/gap_with_opti_constraints_100_runs_$1_$2
    ./run-experiments_ghost.sh $1 $2 >> scripts/ghost/xp_20th_of_June/gap_with_opti_constraints_100_runs_$1_$2
done
