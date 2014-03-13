#!/bin/bash
cd src
g++ -g -std=c++11 main.cpp building.cpp constraint.cpp grid.cpp tools.cpp -o ../bin/wallin
cd ..
valgrind --leak-check=full --show-reachable=yes ./bin/wallin


