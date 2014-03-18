#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const std::set<Constraint*>& setConstraints, 
		  const std::vector<std::shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : setConstraints(setConstraints), 
      vecBuildings(vecBuildings), 
      variableCost( std::vector<double>( vecBuildings.size, 0. ) ),
      tabooList( std::vector<int>( vecBuildings.size, 0 ) ),
      grid(grid)
  { 
    reset();

    // for( auto b : vecBuildings )
    //   mapBuildings[b->getId()] = b;
  }

  void Solver::reset()
  {
    for( auto b : vecBuildings )
      b->setPos( grid.randomPos( *b ) );
    updateConstraints( setConstraints, grid );
  }

  void Solver::move( std::shared_ptr<Building> building, int newPosition )
  {
    grid.clean( *building );
    building->setPos( newPosition );
    grid.add( *building );
    updateConstraints( setConstraints, grid );
  }

  double Solver::solve( double timeout )
  {
    std::chrono::duration<double> elapsedTime;
    std::chrono::time_point<std::chrono::system_clock> start, now;
    start = std::chrono::system_clock::now();

    double bestGlobalCost = std::numeric_limits<double>::max();
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost = std::numeric_limits<double>::max();
    int    bestPosition;

    do 
    {
      now = std::chrono::system_clock::now();
      elapsedTime = now - start;
      
      if( bestGlobalCost == std::numeric_limits<double>::max() )
      {
	currentCost = 0.;
	for( auto c : setConstraints )
	  currentCost += c->cost( variableCost );
	
	if( currentCost < bestGlobalCost )
	  bestGlobalCost = currentCost;
	else
	{
	  reset();
	  continue;
	}
      }

      // Update taboo list
      std::for_each( tabooList.begin(), tabooList.end(), [](int v){ if(v) --v; } );

      // Here, we look at neighbor configurations with the lowest cost.
      std::set<double> worstBuildings;
      double worstVariableCost = 0;
      for( int i = 0; i < variableCost.size(); i++ )
      {
	if( tabooList[i] == 0 )
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
      }
      
      // b is one of the most misplaced building.
      int worstBuildingId = worstBuildings[ randomVar.getRandNum( worstBuildings.size() ) ];
      // Building oldBuilding = *(mapBuildings[ worstBuildingId ]);
      std::shared_ptr<Building> oldBuilding = vecBuildings[ worstBuildingId ];
      Building newBuilding;
      
      // get possible positions for oldBuilding.
      std::set<int> possiblePositions = grid.possiblePos( *oldBuilding );
      for(auto pos : possiblePositions )
      {
	newBuilding.setPos( pos );      
	estimatedCost = 0.;

	// std::for_each( setConstraints.begin(), 
	// 	       setConstraints.end(), 
	// 	       [&](Constraint *c){ estimatedCost += c->simulateCost( *oldBuilding, newBuilding ); });

	for( auto c : setConstraints )
	  estimatedCost += c->simulateCost( *oldBuilding, newBuilding );

	if( estimatedCost < bestEstimatedCost )
	{
	  bestEstimatedCost = estimatedCost;
	  bestPosition = pos;
	}
      }

      currentCost = bestEstimatedCost;

      if( bestEstimatedCost < bestGlobalCost )
      {
	bestGlobalCost = bestEstimatedCost;
	move( oldBuilding, bestPosition );
      }
      else // local minima
      {
	tabooList[ worstBuildingId ] = 5;
      }
           
    } while( bestGlobalCost != 0. && elapsedTime.count() < timeout );

    std::cout << "Elapsed time: " << elapsedTime.count() << std::endl
	      << "Global cost: " << bestGlobalCost << std::endl
	      << "Grids:" << grid << std::endl;

    return bestGlobalCost;
  }
}
