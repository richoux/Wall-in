#!/bin/bash

g++ -fPIC -std=c++0x -O4 -DNDEBUG -I./or-tools/include -DARCH_K8 -Wno-deprecated -DUSE_CBC -DUSE_CLP -c $1.cpp -o $1.o

g++ -fPIC -std=c++0x -O4 -DNDEBUG -I./or-tools/include -DARCH_K8 -Wno-deprecated -DUSE_CBC -DUSE_CLP $1.o -Wl,-rpath ./or-tools/lib -L./or-tools/lib -lortools -lz -lrt -lpthread -o $1
