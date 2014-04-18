#pragma once

//#include <set>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <chrono>
#include <ctime>
#include <limits>
#include <algorithm>
#include <functional>
#include <cassert>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"
#include "tools.hpp"
#include "random.hpp"

using namespace std;

namespace wallin
{
  class Solver
  {
  public:
    Solver( const vector< shared_ptr<Constraint> >&, 
	    const vector<shared_ptr<Building> >&, 
	    const Grid& );
    Solver(const Solver&) = default;
    Solver(Solver&&) = default;
    Solver& operator=(const Solver&) = default;
    Solver& operator=(Solver&&) = default;
    ~Solver() = default;

    double solve( double );
    
  private:
    void reset();
    void move( shared_ptr<Building>&, int );
    set< shared_ptr<Building> > getNecessaryBuildings() const;

    vector< shared_ptr<Constraint> > vecConstraints;
    vector<shared_ptr<Building> > vecBuildings;
    vector<double> variableCost;
    Grid grid;
    vector<int> tabooList;
    Random randomVar;
  };
}
