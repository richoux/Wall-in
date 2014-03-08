#include "../include/constraint.hpp"

namespace wallin
{
  
  Constraint::Constraint(std::vector< std::shared_ptr<Building> >& variables, Grid& grid) noexcept
  : variables( variables ),
    grid( grid )
  { }
  
  Constraint::~Constraint() { }

  
  //Overlap
  Overlap::Overlap(std::vector< std::shared_ptr<Building> >& variables, Grid& grid) noexcept
    : Constraint(variables, grid)
  { }

  double Overlap::cost() const
  {
    // version 1: 1 failure = 1 cost
    // return double( grid.failures().size() );

    // version 2: 1 conflict = 1 cost (may have several conflicts into one failure)
    double conflicts = 0.;

    for( auto failures : grid.failures() )
    {
      int nbConflict = failures.second.size() - 1;
      if( nbConflict > 0 )
	conflicts += nbConflict;
    }

    return conflicts;    
  }

  std::ostream& operator<<( std::ostream& os, const Constraint& c )
  {
    os << "Type: " <<  typeid(c).name() << std::endl;

    for(auto v : c.variables)
      os << (*v) << std::endl;

    return os << c.grid << std::endl
	      << "Cost: " << c.cost() << std::endl;
  }

}
