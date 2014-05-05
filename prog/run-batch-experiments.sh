if [ "$#" -ne 3 ]; then
  echo "Usage: $0 time_limit attempts iterations" >&2
  exit 1
fi
for (( c=1; c<=$3; c++ ))
do
   ./run-experiments.sh $1 $2 > tmp.txt
   bin/processresults tmp.txt | tail -1
done