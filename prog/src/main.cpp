#include <iostream>
#include <vector>
#include <memory>

#include "../include/building.hpp"
#include "../include/constraint.hpp"
#include "../include/grid.hpp"

using namespace wallin;

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

  Grid grid( 6, 8, unbuildables );
  
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
  std::cout << overlap << std::endl;
  std::cout << buildable << std::endl;

  grid.clear( *s );
  overlap.update( grid );
  buildable.update( grid );
  std::cout << overlap << std::endl;
  std::cout << buildable << std::endl;

  s->setPos( grid.mat2lin(4, 5) );
  grid.add( *s );
  overlap.update( grid );
  buildable.update( grid );
  std::cout << overlap << std::endl;
  std::cout << buildable << std::endl;
}
