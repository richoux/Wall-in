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

}
