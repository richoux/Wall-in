#pragma once

#include <algorithm>
#include <set>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

namespace wallin
{
  void updateConstraints( const std::set<Constraint*>&, const Grid& );
  void printConstraints( const std::set<Constraint*>& );
}
