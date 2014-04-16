#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const std::vector< shared_ptr<Constraint> >& vecConstraints, 
		  const std::vector<std::shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : vecConstraints(vecConstraints), 
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
    updateConstraints( vecConstraints, grid );
  }

  void Solver::move( std::shared_ptr<Building>& building, int newPosition )
  {
    grid.clear( *building );
    building->setPos( newPosition );
    grid.add( *building );
    updateConstraints( vecConstraints, grid );
  }

  double Solver::solve( double timeout )
  {
    std::chrono::duration<double,std::milli> elapsedTime;
    std::chrono::time_point<std::chrono::system_clock> start, now;
    start = std::chrono::system_clock::now();

    // to time simulateCost and cost functions
    std::chrono::duration<double,std::milli> timeSimCost(0);
    std::chrono::duration<double,std::milli> timeCost(0);
    std::chrono::time_point<std::chrono::system_clock> startSimCost, startCost;
 
    // int sizeGrid = grid.getNberRows() * grid.getNberCols() + 1; // + 1 for the "position -1" outside the grid
    // std::vector< std::vector< double > > vecConstraintsCosts( vecConstraints.size(), std::vector<double>( vecBuildings.size(), 0. ) );
    // std::vector< double >		 vecGlobalCosts( std::vector<double>( sizeGrid, -1 ) );
    // std::vector< std::vector< double > > vecVarSimCosts( std::vector< std::vector<double> >( sizeGrid, std::vector<double>( vecBuildings.size(), 0. ) ) );

    double bestGlobalCost = std::numeric_limits<double>::max();
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost = std::numeric_limits<double>::max();
    int    bestPosition;

    std::vector<double> worstBuildings;
    double worstVariableCost;
    int worstBuildingId;

    std::shared_ptr<Building> oldBuilding;
    std::vector<int> possiblePositions;
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

	// time costs
	//startCost = std::chrono::system_clock::now();
	
	for( auto c : vecConstraints )
	  currentCost += c->cost( variableCost );

	//timeCost = std::chrono::system_clock::now() - startCost;
	
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
      // std::fill( bestSimCost.begin(), bestSimCost.end(), 0. );

      // for( int i = 0; i < vecConstraints.size(); ++i )
      // 	vecConstraintsCosts[i] = vecConstraints[i]->simulateCost( *oldBuilding, possiblePositions, vecVarSimCosts );

      // std::fill( vecGlobalCosts.begin(), vecGlobalCosts.end(), -1 );
      
      // // sum all numbers in the vector vecConstraintsCosts[i] and put it into vecGlobalCosts[i] 
      // for( auto v : vecConstraintsCosts )
      // 	std::transform( vecGlobalCosts.begin(), 
      // 			vecGlobalCosts.end(), 
      // 			v.begin(), 
      // 			vecGlobalCosts.begin(), 
      // 			std::plus<double>() );
      
      // // replace all negative numbers by the max value for double
      // std::replace_if( vecGlobalCosts.begin(), 
      // 		       vecGlobalCosts.end(), 
      // 		       std::bind( std::less<double>(), std::placeholders::_1, 0. ), 
      // 		       std::numeric_limits<double>::max() );

      // // look for the first smallest cost
      // for( int i = 0; i < vecGlobalCosts.size(); ++i )
      // 	if( vecGlobalCosts[i] < bestEstimatedCost
      // 	    || ( vecGlobalCosts[i] == bestEstimatedCost //heuristics: better to take i=-1 or the nearest position from the target tile. 
      // 		 && ( i == 0 || grid.distanceToTarget( i - 1 ) < grid.distanceToTarget( bestPosition ) ) ) )
      // 	{
      // 	  bestEstimatedCost = vecGlobalCosts[i];
      // 	  bestPosition = i - 1;
      // 	  bestSimCost = vecVarSimCosts[i];
      // 	}

      for(auto pos : possiblePositions )
      {
      	estimatedCost = 0.;
      	std::fill( varSimCost.begin(), varSimCost.end(), 0. );
      
      	// time simulateCost
      	//startSimCost = std::chrono::system_clock::now();

      	for( auto c : vecConstraints )
      	  estimatedCost += c->simulateCost( *oldBuilding, pos, varSimCost );

      	//timeSimCost += std::chrono::system_clock::now() - startSimCost;

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
      double cost;
      NoGapsFinalize ngf( vecBuildings, grid );

      do
      {
    	for( auto b : vecBuildings )
	  if( ! grid.isStartingOrTargetTile( b->getId() ) )
	  {
	    change = false;
	    if( b->isOnGrid() )
	    {
	      cost = 0.;
	      std::fill( varSimCost.begin(), varSimCost.end(), 0. );
	      
	      cost = ngf.simulateCost( *b, -1, varSimCost );
	      
	      if( cost == 0. )
	      {
		grid.clear( *b );
		b->setPos( -1 );
		grid.add( *b );
		ngf.update( grid );
		change = true;
	      }	  
	    }
	  }
      } while( change );
    }

    std::cout << "Grids:" << grid << std::endl
	      << "Elapsed time: " << elapsedTime.count() << std::endl
      //<< "Elapsed time to simulate cost: " << timeSimCost.count() << std::endl
      //<< "Elapsed time to cost: " << timeCost.count() << std::endl
	      << "Global cost: " << bestGlobalCost << std::endl;

    // print cost for each constraint
    for( auto c : vecConstraints )
    {
      std::fill( varSimCost.begin(), varSimCost.end(), 0. );
      std::cout << "Cost of " << typeid(*c).name() << ": " << c->cost( varSimCost ) << " [";

      for( auto v : varSimCost )
	std::cout << " " << v;

      std::cout << " ]" << std::endl;
    }      
    
    std::cout << std::endl;
    
    return bestGlobalCost;
  }
}
