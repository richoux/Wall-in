#!/bin/bash
cd src
g++ -g -std=c++11 main.cpp building.cpp constraint.cpp grid.cpp tools.cpp solver.cpp random.cpp -o ../bin/wallin
cd ..
