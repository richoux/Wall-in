#pragma once

#include <set>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

namespace wallin
{
  class Solver
  {
  public:
    Solver( const std::set<Constraint*>&, 
	    const std::vector<std::shared_ptr<Building> >&, 
	    const Grid& ) noexcept;

    int solve( double );
    
  private:
    std::set<Constraint*> setConstraints;
    std::vector<std::shared_ptr<Building> > vecBuildings;
    Grid grid;
  };
}
