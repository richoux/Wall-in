#include "../include/constraint.hpp"

namespace wallin
{
  
  Constraint::Constraint(std::vector< std::shared_ptr<Building> >& variables, Grid& grid) noexcept
  : variables( variables ),
    grid( grid )
  { }
  
  Constraint::~Constraint() { }

  
  //IsBuildable
  IsBuildable::IsBuildable(std::vector< std::shared_ptr<Building> >& variables, Grid& grid) noexcept
    : Constraint(variables, grid)
  { }

  double IsBuildable::cost() const
  {
    
  }

  std::ostream& operator<<( std::ostream& os, const Constraint& c )
  {
    os << "Type: " <<  typeid(c).name() << std::endl;

    for(auto v : c.variables)
      os << (*v) << std::endl;

    return os << c.grid;
  }

}
