#!/bin/bash
#g++ -gstabs -std=c++11 experiments/main.cpp src/building.cpp src/constraint.cpp src/grid.cpp src/tools.cpp src/solver.cpp src/random.cpp src/objective.cpp -o bin/experiments
g++ -std=c++11 -Ofast -DNDEBUG experiments/main.cpp src/building.cpp src/constraint.cpp src/grid.cpp src/tools.cpp src/solver.cpp src/random.cpp src/objective.cpp -o bin/experiments
g++ -std=c++11 -Ofast -DNDEBUG experiments/processrawdata.cpp -o bin/processrawdata
g++ -std=c++11 -Ofast -DNDEBUG experiments/processresults.cpp -o bin/processresults

