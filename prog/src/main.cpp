#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

#include "../include/building.hpp"
#include "../include/constraint.hpp"
#include "../include/grid.hpp"
#include "../include/tools.hpp"
#include "../include/terran.hpp"
#include "../include/solver.hpp"

using namespace wallin;

int main(int argc, char **argv)
{
  std::vector< std::pair<int, int> > unbuildables 
  { 
    std::make_pair<int, int>(7, 12), 
    std::make_pair<int, int>(7, 13), 
    std::make_pair<int, int>(7, 14), 
    std::make_pair<int, int>(7, 15), 
    std::make_pair<int, int>(8, 10), 
    std::make_pair<int, int>(8, 11), 
    std::make_pair<int, int>(8, 12), 
    std::make_pair<int, int>(8, 13), 
    std::make_pair<int, int>(8, 14), 
    std::make_pair<int, int>(8, 15), 
    std::make_pair<int, int>(9, 10), 
    std::make_pair<int, int>(9, 11), 
    std::make_pair<int, int>(9, 12), 
    std::make_pair<int, int>(9, 13), 
    std::make_pair<int, int>(9, 14), 
    std::make_pair<int, int>(9, 15), 
    std::make_pair<int, int>(10, 8), 
    std::make_pair<int, int>(10, 9), 
    std::make_pair<int, int>(10, 10), 
    std::make_pair<int, int>(10, 11), 
    std::make_pair<int, int>(10, 12), 
    std::make_pair<int, int>(10, 13), 
    std::make_pair<int, int>(10, 14), 
    std::make_pair<int, int>(10, 15), 
    std::make_pair<int, int>(11, 8), 
    std::make_pair<int, int>(11, 9), 
    std::make_pair<int, int>(11, 10), 
    std::make_pair<int, int>(11, 11), 
    std::make_pair<int, int>(11, 12), 
    std::make_pair<int, int>(11, 13), 
    std::make_pair<int, int>(11, 14), 
    std::make_pair<int, int>(11, 15) 
  };

  Grid grid( 12, 16, unbuildables, 11, 7, 6, 15 );

  std::vector<std::shared_ptr<Building> > vec	= makeTerranBuildings();
  std::set< Constraint* > setConstraints	= makeTerranConstraints( vec, grid );
  
  Solver solver( setConstraints, vec, grid );
  solver.solve( 16 );    
}
