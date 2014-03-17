#pragma once

#include <set>
#include <vector>
#include <map>
#include <memory>
#include <chrono>
#include <ctime>
#include <limits>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"
#include "tools.hpp"

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
    void reset();

    std::set<Constraint*> setConstraints;
    std::vector<std::shared_ptr<Building> > vecBuildings;
    std::vector<int> variableCost;
    Grid grid;
    std::map<int, std::shared_ptr<Building> > mapBuildings;
    Random randomVar;
  };
}
