#!/bin/bash
cd src
g++ -std=c++11 main.cpp building.cpp constraint.cpp grid.cpp tools.cpp solver.cpp -o ../bin/wallin
cd ..

