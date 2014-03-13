#!/bin/bash
cd src
g++ -std=c++11 main.cpp building.cpp constraint.cpp grid.cpp tools.cpp -o ../bin/wallin
cd ..

