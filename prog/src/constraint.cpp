#include "../include/constraint.hpp"

namespace wallin
{
  
  Constraint::Constraint(const std::vector< std::shared_ptr<Building> >& variables, const Grid& grid) noexcept
  : variables( variables ),
    grid( grid )
  { }
  
  Constraint::~Constraint() { }

  std::ostream& operator<<( std::ostream& os, const Constraint& c )
  {
    os << "Type: " <<  typeid(c).name() << std::endl;

    for(auto v : c.variables)
      os << (*v) << std::endl;

    return os << c.grid << std::endl
	      << "Cost: " << c.cost() << std::endl;
  }

  /***********/
  /* Overlap */
  /***********/
  Overlap::Overlap(const std::vector< std::shared_ptr<Building> >& variables, const Grid& grid) noexcept
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
      if( nbConflict > 0 && failures.second.find( "###" ) == std::string::npos )
	conflicts += nbConflict;
    }

    return conflicts;    
  }

  /*************/
  /* Buildable */
  /*************/
  Buildable::Buildable(const std::vector< std::shared_ptr<Building> >& variables, const Grid& grid) noexcept
    : Constraint(variables, grid)
  { }

  double Buildable::cost() const
  {
    // count number of buildings misplaced on unbuildable tiles (denoted by ###)
    double conflicts = 0.;

    for( auto failures : grid.failures() )
    {
      if( failures.second.find( "###" ) != std::string::npos )
	conflicts += failures.second.size() - 3;
    }

    return conflicts;    
  }

  /**********/
  /* NoGaps */
  /**********/
  NoGaps::NoGaps(const std::vector< std::shared_ptr<Building> >& variables, const Grid& grid) noexcept
    : Constraint(variables, grid)
  { }

  double NoGaps::cost() const
  {
    // cost = |buildings with one neighbor| - 1 + |buildings with no neighbors|
    double conflicts = 0.;
    int oneNeighbor = 0;
    int nberNeighbors;

    for( auto building : variables )
    {
      nberNeighbors = countAround( *building );
      if( nberNeighbors == 0 )
	conflicts++;
      else
      {
	if( nberNeighbors == 1 && oneNeighbor++ > 2 )
	  conflicts++;
      }
    }

    return conflicts;    
  }

  int NoGaps::countAround( const Building& b ) const
  {
    std::pair<int, int> coordinates = grid.lin2mat( b.getPosition() );

    int top = coordinates.first;
    int right = coordinates.second + b.getLength();
    int bottom = coordinates.first + b.getHeight();
    int left = coordinates.second;

    int counter = 0;

    for(auto other : variables )
    {
      if( other->getId() != b.getId() )
      {
	std::pair<int, int> xyOther = grid.lin2mat( other->getPosition() );
	int otherTop = xyOther.first;
	int otherRight = xyOther.second + other->getLength();
	int otherBottom = xyOther.first + other->getHeight();
	int otherLeft = xyOther.second;
	
	if(  ( top == otherBottom + 1 && ( otherRight >= left && otherRight <= right + other->getLength() ) )
	  || ( right == otherLeft - 1 && ( otherBottom >= top - 1 && otherBottom <= bottom + 1 + other->getHeight() ) )
	  || ( bottom == otherTop - 1 && ( otherRight >= left && otherRight <= right + other->getLength() ) )
	  || ( left == otherRight + 1 && ( otherBottom >= top - 1 && otherBottom <= bottom + 1 + other->getHeight() ) ) )
	{
	  counter++;
	}
      }
    }
  }

}
