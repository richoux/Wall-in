#!/bin/bash
cd src
#g++ -gstabs -D_GLIBCXX_DEBUG -std=c++11 main.cpp building.cpp constraint.cpp grid.cpp tools.cpp solver.cpp random.cpp -o ../bin/wallin
g++ -gstabs -std=c++11 main.cpp building.cpp constraint.cpp grid.cpp tools.cpp solver.cpp random.cpp -o ../bin/wallin
cd ..
