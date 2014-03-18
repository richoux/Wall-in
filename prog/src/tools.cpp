#include "../include/tools.hpp"

namespace wallin
{
  void updateConstraints( const std::set<Constraint*>& setConstraints, const Grid& grid )
  {
    std::for_each( setConstraints.begin(), setConstraints.end(), [&](Constraint *c){ c->update( grid ); });
  }

  void printConstraints( const std::set<Constraint*>& setConstraints )
  {
    std::for_each( setConstraints.begin(), setConstraints.end(), [&](Constraint *c){ std::cout << *c << std::endl; });
  }

  void addAllInGrid( const std::vector<std::shared_ptr<Building> >& vec, Grid& grid )
  {
    std::for_each( vec.begin(), vec.end(), [&](std::shared_ptr<Building> b){ grid.add(*b); });
  }
}
