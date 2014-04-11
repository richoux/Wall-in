#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const std::set< shared_ptr<Constraint> >& setConstraints, 
		  const std::vector<std::shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : setConstraints(setConstraints), 
      vecBuildings(vecBuildings), 
      variableCost( std::vector<double>( vecBuildings.size(), 0. ) ),
      grid(grid),
      tabooList( std::vector<int>( vecBuildings.size(), 0 ) )
  { 
    reset();
  }

  void Solver::reset()
  {
    int xPos;
    int yPos;
    for( auto b : vecBuildings )
    {
      // 1 chance over 3 to be placed on the grid
      int r = randomVar.getRandNum(3);
      if( r == 0)
      {
	xPos = randomVar.getRandNum( grid.getNberRows() - b->getLength() );
	yPos = randomVar.getRandNum( grid.getNberCols() - b->getHeight() );
	b->setPos( grid.mat2lin( xPos, yPos ) );

	grid.add( *b );
      }
      else
	b->setPos( -1 );
    }
    updateConstraints( setConstraints, grid );
  }

  void Solver::move( std::shared_ptr<Building>& building, int newPosition )
  {
    grid.clear( *building );
    building->setPos( newPosition );
    grid.add( *building );
    updateConstraints( setConstraints, grid );
  }

  double Solver::solve( double timeout )
  {
    std::chrono::duration<double,std::milli> elapsedTime;
    std::chrono::time_point<std::chrono::system_clock> start, now;
    start = std::chrono::system_clock::now();

    double bestGlobalCost = std::numeric_limits<double>::max();
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost = std::numeric_limits<double>::max();
    int    bestPosition;

    std::vector<double> worstBuildings;
    double worstVariableCost;
    int worstBuildingId;

    std::shared_ptr<Building> oldBuilding;
    std::set<int> possiblePositions;
    std::vector<double> varSimCost( std::vector<double>( vecBuildings.size(), 0. ) );
    std::vector<double> bestSimCost( std::vector<double>( vecBuildings.size(), 0. ) );

    int tour = 1;
    int test = 1;

    do 
    {
      now = std::chrono::system_clock::now();
      
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
      for( int i = 0; i < tabooList.size(); ++i )
	if( tabooList[i] != 0 )
	  --tabooList[i];

      // Here, we look at neighbor configurations with the lowest cost.
      worstBuildings.clear();
      worstVariableCost = 0;
      for( int i = 0; i < variableCost.size(); ++i )
      {
	if( tabooList[i] == 0 )
	{
	  if( worstVariableCost < variableCost[i] )
	  {
	    worstVariableCost = variableCost[i];
	    worstBuildings.clear();
	    worstBuildings.push_back( i );
	  }
	  else 
	    if( worstVariableCost == variableCost[i] )
	      worstBuildings.push_back( i );	  
	}
      }
      
      // b is one of the most misplaced building.
      worstBuildingId = worstBuildings[ randomVar.getRandNum( worstBuildings.size() ) ];

      // Building oldBuilding = *(mapBuildings[ worstBuildingId ]);
      oldBuilding = vecBuildings[ worstBuildingId ];
      
      // get possible positions for oldBuilding.
      possiblePositions = grid.possiblePos( *oldBuilding );

      // variable simulated costs
      std::fill( bestSimCost.begin(), bestSimCost.end(), 0. );
      for(auto pos : possiblePositions )
      {
	estimatedCost = 0.;
	std::fill( varSimCost.begin(), varSimCost.end(), 0. );
      
	for( auto c : setConstraints )
	  estimatedCost += c->simulateCost( *oldBuilding, pos, varSimCost );

	if( estimatedCost < bestEstimatedCost 
	    || ( estimatedCost == bestEstimatedCost //heuristics: better to take pos=-1 or the nearest position from the target tile. 
		 && ( pos == -1 || grid.distanceToTarget( pos ) < grid.distanceToTarget( bestPosition ) ) ) )
	{
	  bestEstimatedCost = estimatedCost;
	  bestPosition = pos;
	  bestSimCost = varSimCost;
	}
      }

      currentCost = bestEstimatedCost;

      if( bestEstimatedCost < bestGlobalCost )
      {
	bestGlobalCost = bestEstimatedCost;
	variableCost = bestSimCost;
	move( oldBuilding, bestPosition );
      }
      else // local minima
	tabooList[ worstBuildingId ] = 5;

      elapsedTime = now - start;
    } while( bestGlobalCost != 0. && elapsedTime.count() < timeout );

    // remove useless buildings
    if( bestGlobalCost == 0 )
    {
      bool change;
      do
      {
	for( auto b : vecBuildings )
	{
	  change = false;
	  if( b->isOnGrid() )
	  {
	    estimatedCost = 0.;
	    std::fill( varSimCost.begin(), varSimCost.end(), 0. );
	    
	    for( auto c : setConstraints )
	      estimatedCost += c->simulateCost( *b, -1, varSimCost );
	    
	    if( estimatedCost == 0. )
	    {
	      move( b, -1 );
	      change = true;
	    }	  
	  }
	}
      } while( change );
    }

    std::cout << "Grids:" << grid << std::endl
	      << "Elapsed time: " << elapsedTime.count() << std::endl
	      << "Global cost: " << bestGlobalCost << std::endl;

    return bestGlobalCost;
  }
}
