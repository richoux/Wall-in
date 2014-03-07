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

  b->setPos(9);

  Grid grid(5,8);
  grid.add(0, 0, (*b) );
  grid.add(1, 3, (*f) );
  grid.add(2, 1, (*s) );

  std::vector<std::shared_ptr<Building> > vec;
  vec.push_back( b );
  vec.push_back( f );
  vec.push_back( s );

  IsBuildable isbuildable( vec, grid );
  std::cout << isbuildable << std::endl;
    
}
