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

  Grid grid(6,8);

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

  IsBuildable isbuildable( vec, grid );
  std::cout << isbuildable << std::endl;
    
  grid.clear( *s );
  isbuildable.update( grid );
  std::cout << isbuildable << std::endl;

  s->setPos( grid.mat2lin(3, 0) );
  grid.add( *s );
  isbuildable.update( grid );
  std::cout << isbuildable << std::endl;
}
