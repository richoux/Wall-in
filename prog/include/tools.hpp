#pragma once

#include <algorithm>
#include <set>
#include <vector>
#include <memory>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

namespace wallin
{
  void updateConstraints( const std::set< shared_ptr<Constraint> >&, const Grid& );
  void printConstraints( const std::set< shared_ptr<Constraint> >& );
  void addAllInGrid( const std::vector<std::shared_ptr<Building> >&, Grid& );
}
