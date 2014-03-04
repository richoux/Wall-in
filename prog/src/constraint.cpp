#include "../include/constraint.hpp"

namespace wallin
{
  
  Constraint::Constraint(std::vector< std::shared_ptr<Building> > variables, int row, int col)
    : variables(variables),
      grid( std::vector< std::vector< std::string > >(col, std::vector< std::string > >(row, "") )
  { }
  
  Constraint::~Constraint() { }

  
  //IsBuildable
  IsBuildable::IsBuildable(std::vector< std::shared_ptr<Building> > variables, int row, int col)
    : Constraint(variables, row, col)
  { }

  double IsBuildable::cost() const
  {
    
  }
}
