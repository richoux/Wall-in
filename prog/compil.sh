#!/bin/bash
cd src
g++ -std=c++11 -Ofast -DNDEBUG main.cpp building.cpp constraint.cpp grid.cpp tools.cpp solver.cpp random.cpp -o ../bin/wallin
cd ..

