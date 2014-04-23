#!/bin/bash
g++ -std=c++11 -Ofast -DNDEBUG src/main.cpp src/building.cpp src/constraint.cpp src/grid.cpp src/tools.cpp src/solver.cpp src/random.cpp src/objective.cpp -o bin/wallin

