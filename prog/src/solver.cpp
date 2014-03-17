#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const std::set<Constraint*>& setConstraints, 
		  const std::vector<std::shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : setConstraints(setConstraints), 
      vecBuildings(vecBuildings), 
      grid(grid)
  { 
    reset();

    for( auto b : vecBuildings )
      mapBuildings[b->getId()] = b;
  }

  void Random::reset()
  {
    for( auto b : vecBuildings )
      b->setPos( grid.randomPos( *b ) );
    updateConstraints( setConstraints, grid );
  }

  int Solver::solve( double timeout )
  {
    std::chrono::duration<double> elapsedTime;
    std::chrono::time_point<std::chrono::system_clock> start, now;
    start = std::chrono::system_clock::now();

    double globalCost = std::numeric_limits<double>::max();
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost;

    do {
      now = std::chrono::system_clock::now();
      elapsedTime = now - start;
      
      if( globalCost == std::numeric_limits<double>::max() )
      {
	currentCost = 0.;
	for( auto c : setConstraints )
	  currentCost += c->cost( variableCost );
	
	if( currentCost < globalCost )
	  globalCost = currentCost;
	else
	{
	  reset();
	  continue;
	}
      }

      // Here, we look at neighbor configurations with the lowest cost.
      // var cost in double?
      std::set<int> worstBuildings;
      int worstVariableCost = 0;
      for( int i = 0; i < variableCost.size(); i++ )
      {
	if( worstVariableCost < variableCost[i] )
	{
	  worstVariableCost = variableCost[i];
	  worstBuildings.clear();
	  worstBuildings.insert( i );
	}
	else 
	  if( worstVariableCost == variableCost[i] )
	    worstBuildings.insert( i );	  
      }
      
      // b is one of the most misplaced building.
      Building oldBuilding = mapBuildings[ worstBuildings[ randomVar.getRandNum( worstBuildings.size() ) ] ];
      Building newBuilding;
      
      // get possible positions for oldBuilding.
      std::set<int> possiblePositions = grid.possiblePos( oldBuilding );
      for(auto pos : possiblePositions )
      {
	newBuilding.setPos( pos );      
	
      }

      
    } while( globalCost != 0. && elapsedTime.count() < timeout );
  }
}
