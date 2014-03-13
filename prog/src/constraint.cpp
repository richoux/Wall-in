#include "../include/constraint.hpp"

namespace wallin
{
  
  Constraint::Constraint(const std::vector< std::shared_ptr<Building> >& variables, const Grid& grid) noexcept
  : variables( variables ),
    grid( grid )
  { 
    for( auto b : variables )
      mapBuildings[b->getId()] = b;
  }
  
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
      nberNeighbors = grid.countAround( *building, variables );
      if( nberNeighbors == 0 )
	conflicts++;
      else
      {
	if( nberNeighbors == 1 && oneNeighbor++ > 2 )
	  conflicts++;
      }
      
      std::cout << building->getShort() << ": " << nberNeighbors << std::endl;
    }

    return conflicts;    
  }

  /***********************/
  /* StartingTargetTiles */
  /***********************/
  StartingTargetTiles::StartingTargetTiles(const std::vector< std::shared_ptr<Building> >& variables, const Grid& grid) noexcept
    : Constraint(variables, grid)
  { }

  double StartingTargetTiles::cost() const
  {
    // no building on one of these two tiles: cost of the tile = 3
    // a building with no or with 2 or more neighbors: cost of the tile = 1
    // two or more buildings on one of these tile: increasing penalties.
    double conflicts = 0.;

    std::set<int> startingBuildings = grid.buildingsAt( grid.getStartingTile() );
    std::set<int> targetBuildings = grid.buildingsAt( grid.getTargetTile() );

    std::shared_ptr<Building> b;
    int neighbors;

    if( startingBuildings.empty() )
      conflicts += 3;
    else
    {
      int penalty = 0;
      for( int bId : startingBuildings )
      {
	b = mapBuildings.at(bId);
	neighbors = grid.countAround( *b, variables );
	
	if (neighbors != 1)
	  conflicts += 1;

	conflicts += penalty++;
      }
    }

    if( targetBuildings.empty() )
      conflicts += 3;
    else
    {
      int penalty = 0;
      for( int bId : targetBuildings )
      {
	b = mapBuildings.at(bId);
	neighbors = grid.countAround( *b, variables );
	
	if (neighbors != 1)
	  conflicts += 1;

	conflicts += penalty++;	
      }
      
    }
    
    return conflicts;    
  }

}
