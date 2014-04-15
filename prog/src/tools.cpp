#include "../include/tools.hpp"

namespace wallin
{
  void updateConstraints( const std::vector< std::shared_ptr<Constraint> >& setConstraints, const Grid& grid )
  {
    std::for_each( setConstraints.begin(), setConstraints.end(), [&]( const std::shared_ptr<Constraint>& c){ c->update( grid ); });
  }

  void printConstraints( const std::vector< std::shared_ptr<Constraint> >& setConstraints )
  {
    std::for_each( setConstraints.begin(), setConstraints.end(), [&]( const std::shared_ptr<Constraint>& c){ std::cout << *c << std::endl; });
  }

  void addAllInGrid( const std::vector<std::shared_ptr<Building> >& vec, Grid& grid )
  {
    std::for_each( vec.begin(), vec.end(), [&](const std::shared_ptr<Building>& b){ grid.add(*b); });
  }
}
