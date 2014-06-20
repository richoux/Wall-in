#!/bin/bash
GHOST=/home/flo/ghost
g++ -std=c++11 -Ofast -DNDEBUG experiments/main_ghost.cpp $GHOST/src/variables/variable.cpp $GHOST/src/variables/building.cpp $GHOST/src/constraints/wallinConstraint.cpp $GHOST/src/domains/wallinDomain.cpp $GHOST/src/objectives/wallinObjective.cpp -o bin/experiments_ghost
g++ -std=c++11 -Ofast -DNDEBUG experiments/processrawdata.cpp -o bin/processrawdata
g++ -std=c++11 -Ofast -DNDEBUG experiments/processresults.cpp -o bin/processresults

