#pragma once

#include <algorithm>
// #include <set>
#include <vector>
#include <memory>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

namespace wallin
{
  void updateConstraints( const std::vector< shared_ptr<Constraint> >&, const Grid& );
  void printConstraints( const std::vector< shared_ptr<Constraint> >& );
  void addAllInGrid( const std::vector<std::shared_ptr<Building> >&, Grid& );
  void clearAllInGrid( const std::vector<std::shared_ptr<Building> >&, Grid& );
}
