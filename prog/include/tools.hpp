#pragma once

#include <algorithm>
#include <set>
#include <vector>
#include <memory>
#include <random>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

namespace wallin
{
  void updateConstraints( const std::set<Constraint*>&, const Grid& );
  void printConstraints( const std::set<Constraint*>& );
  void randomConfiguration( const std::vector<std::shared_ptr<Building> >&, const Grid& );

  class Random
  {
  public:
    Random();
    inline int getRandNum( int limit ) const { return ( numbers(rng) % limit ); } 
  private:
    std::mt19937			rng;
    std::random_device			rd;
    std::uniform_int_distribution<int>  numbers;
  };
}
