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
#include "random.hpp"

namespace wallin
{
  class Solver
  {
  public:
    Solver( const std::set<Constraint*>&, 
	    const std::vector<std::shared_ptr<Building> >&, 
	    const Grid& );
    Solver(const Solver&) = default;
    Solver(Solver&&) = default;
    Solver& operator=(const Solver&) = default;
    Solver& operator=(Solver&&) = default;
    ~Solver() = default;

    double solve( double );
    
  private:
    void reset();
    void move( std::shared_ptr<Building>&, int );

    std::set<Constraint*> setConstraints;
    std::vector<std::shared_ptr<Building> > vecBuildings;
    std::vector<double> variableCost;
    Grid grid;
    std::vector<int> tabooList;
    Random randomVar;
  };
}
