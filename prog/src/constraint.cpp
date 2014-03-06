#include "../include/constraint.hpp"

namespace wallin
{
  
  Constraint::Constraint(std::vector< std::shared_ptr<Building>> variables, std::shared_ptr<Grid> grid) noexcept
    : variables( variables ),
      grid( grid )
  { }
  
  Constraint::~Constraint() { }

  
  //IsBuildable
  IsBuildable::IsBuildable(std::vector< std::shared_ptr<Building>> variables, std::shared_ptr<Grid> grid) noexcept
    : Constraint(variables, grid)
  { }

  double IsBuildable::cost() const
  {
    
  }
}
