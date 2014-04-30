#!/bin/bash
#g++ -gstabs -D_GLIBCXX_DEBUG -std=c++11 src/main.cpp src/building.cpp src/constraint.cpp src/grid.cpp src/tools.cpp src/solver.cpp src/random.cpp src/objective.cpp -o bin/wallin
g++ -gstabs -std=c++11 src/main.cpp src/building.cpp src/constraint.cpp src/grid.cpp src/tools.cpp src/solver.cpp src/random.cpp src/objective.cpp -o bin/wallin
