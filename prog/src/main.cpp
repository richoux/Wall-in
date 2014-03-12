#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

#include "../include/building.hpp"
#include "../include/constraint.hpp"
#include "../include/grid.hpp"

using namespace wallin;

void updateConstraints( const std::set<Constraint*>& setConstraints, const Grid& grid )
{
  std::for_each( setConstraints.begin(), setConstraints.end(), [&](Constraint *c){ c->update( grid ); });
}

void printConstraints( const std::set<Constraint*>& setConstraints )
{
  std::for_each( setConstraints.begin(), setConstraints.end(), [&](Constraint *c){ std::cout << *c << std::endl; });
}

int main(int argc, char **argv)
{
  std::shared_ptr<Building> b = std::make_shared<Barracks>( );
  std::shared_ptr<Building> f = std::make_shared<Factory>( );
  std::shared_ptr<Building> s = std::make_shared<SupplyDepot>( );

  std::vector< std::pair<int, int> > unbuildables 
  { 
    std::make_pair<int, int>(5, 5), 
    std::make_pair<int, int>(5, 6), 
    std::make_pair<int, int>(5, 7) 
  };

  Grid grid( 6, 8, unbuildables, 5, 0, 0, 7 );
  
  b->setPos( grid.mat2lin(0, 0) );
  f->setPos( grid.mat2lin(1, 3) );
  s->setPos( grid.mat2lin(2, 1) );

  grid.add( *b );
  grid.add( *f );
  grid.add( *s );

  std::vector<std::shared_ptr<Building> > vec;
  vec.push_back( b );
  vec.push_back( f );
  vec.push_back( s );

  Overlap overlap( vec, grid );
  Buildable buildable( vec, grid );
  NoGaps noGaps( vec, grid );
  
  std::set<Constraint*> setConstraints;
  setConstraints.insert( &overlap );
  setConstraints.insert( &buildable );
  setConstraints.insert( &noGaps );  
  
  printConstraints( setConstraints );

  grid.clear( *s );
  updateConstraints( setConstraints, grid );
  printConstraints( setConstraints );

  s->setPos( grid.mat2lin(4, 5) );
  grid.add( *s );
  updateConstraints( setConstraints, grid );
  printConstraints( setConstraints );

  grid.clear( *f );
  grid.clear( *s );
  f->setPos( grid.mat2lin(1, 4) );
  s->setPos( grid.mat2lin(4, 0) );
  grid.add( *f );
  grid.add( *s );
  updateConstraints( setConstraints, grid );
  printConstraints( setConstraints );
  
}
