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
  std::vector<std::shared_ptr<Building> > vec = makeTerranBuildings();

  std::vector< std::pair<int, int> > unbuildables 
  { 
    std::make_pair<int, int>(5, 5), 
    std::make_pair<int, int>(5, 6), 
    std::make_pair<int, int>(5, 7) 
  };

  Grid grid( 6, 8, unbuildables, 5, 0, 0, 7 );
  
  // b->setPos( grid.mat2lin(0, 0) );
  // f->setPos( grid.mat2lin(1, 3) );
  // s1->setPos( grid.mat2lin(2, 1) );

  // grid.add( *b );
  // grid.add( *f );
  // grid.add( *s1 );

  Overlap overlap( vec, grid );
  Buildable buildable( vec, grid );
  NoGaps noGaps( vec, grid );
  StartingTargetTiles specialTiles( vec, grid );
  
  std::set<Constraint*> setConstraints;
  setConstraints.insert( &overlap );
  setConstraints.insert( &buildable );
  setConstraints.insert( &noGaps );  
  setConstraints.insert( &specialTiles );  
  
  // printConstraints( setConstraints );

  // grid.clear( *s1 );
  // updateConstraints( setConstraints, grid );
  // printConstraints( setConstraints );
  
  // s1->setPos( grid.mat2lin(4, 5) );
  // grid.add( *s1 );
  // updateConstraints( setConstraints, grid );
  // printConstraints( setConstraints );
  
  // grid.clear( *f );
  // grid.clear( *s1 );
  // f->setPos( grid.mat2lin(1, 4) );
  // s1->setPos( grid.mat2lin(4, 0) );
  // grid.add( *f );
  // grid.add( *s1 );
  // updateConstraints( setConstraints, grid );
  // printConstraints( setConstraints );

  Solver solver( setConstraints, vec, grid );
  solver.solve( 1000 );
    
}
