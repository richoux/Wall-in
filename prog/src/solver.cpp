#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const std::set<Constraint*>& setConstraints, 
		  const std::vector<std::shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : setConstraints(setConstraints), 
      vecBuildings(vecBuildings), 
      variableCost( std::vector<double>( vecBuildings.size(), 0. ) ),
      grid(grid),
      tabooList( std::vector<int>( vecBuildings.size(), 0 ) )
  { 
    reset();

    // for( auto b : vecBuildings )
    //   mapBuildings[b->getId()] = b;
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

	// std::cout<< "r:" << r << std::endl;
	// std::cout << "Position for " 
	// 	  << b->getShort() 
	// 	  << "(" << b->getId() << "): " 
	// 	  << b->getPosition() 
	// 	  << " (" << xPos << "," << yPos << ")" << std::endl;      
      }
      else
      {
	b->setPos( -1 );
	// std::cout<< "r:" << r << std::endl;
      }
    }
    updateConstraints( setConstraints, grid );
    
    std::cout << grid << std::endl;
  }

  void Solver::move( std::shared_ptr<Building>& building, int newPosition )
  {
    std::cout << "Move " << building->getShort() 
	      << "(" << building->getId() << ")"
	      << " now at " << building->getPosition()
	      << " to " << newPosition << std::endl;
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

    int tour = 1;
    int test = 1;

    do 
    {
      now = std::chrono::system_clock::now();
      elapsedTime = now - start;
      
      std::cout << "Tour: " << tour++ << std::endl
      		<< "Elapsed time: " << elapsedTime.count() << std::endl
      		<< "Global cost: " << bestGlobalCost << std::endl
      		<< "Grids:" << grid << std::endl;

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

      std::cout << "Taboo List: 0=" << tabooList[0];
      for(int i = 1; i < tabooList.size(); ++i )
      	std::cout << ", " << i << "=" << tabooList[i];
      std::cout << std::endl;

      std::cout << "Var costs:  0=" << variableCost[0];
      for(int i = 1; i < variableCost.size(); ++i )
      	std::cout << ", " << i << "=" << variableCost[i];
      std::cout << std::endl;
      
      // Here, we look at neighbor configurations with the lowest cost.
      std::vector<double> worstBuildings;
      double worstVariableCost = 0;
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
      int worstBuildingId = worstBuildings[ randomVar.getRandNum( worstBuildings.size() ) ];

      // std::cout << "Worst var cost: " <<  worstVariableCost << std::endl;

      // std::cout << "Worstvars: 0=" << worstBuildings[0];
      // for(int i = 1; i < worstBuildings.size(); ++i )
      // 	std::cout << ", " << i << "=" << worstBuildings[i];

      // std::cout << std::endl << "Worst var id: " << worstBuildingId << std::endl;

      // Building oldBuilding = *(mapBuildings[ worstBuildingId ]);
      std::shared_ptr<Building> oldBuilding = vecBuildings[ worstBuildingId ];
      //decltype(*oldBuilding) newBuilding = *oldBuilding;
      
      // get possible positions for oldBuilding.
      std::set<int> possiblePositions = grid.possiblePos( *oldBuilding );

      // variable simulated costs
      std::vector<double> varSimCost( std::vector<double>( vecBuildings.size(), 0. ) );
      std::vector<double> bestSimCost( std::vector<double>( vecBuildings.size(), 0. ) );
      for(auto pos : possiblePositions )
      {
	estimatedCost = 0.;
	std::fill( varSimCost.begin(), varSimCost.end(), 0. );
      
	// std::for_each( setConstraints.begin(), 
	// 	       setConstraints.end(), 
	// 	       [&](Constraint *c){ estimatedCost += c->simulateCost( *oldBuilding, newBuilding ); });

	for( auto c : setConstraints )
	  estimatedCost += c->simulateCost( *oldBuilding, pos, varSimCost );

	// std::cout << "Estimated cost on position " << pos << ": " << estimatedCost << std::endl;

	if( estimatedCost < bestEstimatedCost )
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

	// std::cout << "Elapsed time: " << elapsedTime.count() << std::endl
	// 	  << "Global cost: " << bestGlobalCost << std::endl
	// 	  << "Grids:" << grid << std::endl;
      }
      else // local minima
	tabooList[ worstBuildingId ] = 5;
           
    } while( bestGlobalCost != 0. && elapsedTime.count() < timeout );

    std::cout << "Final costs:" << std::endl;
    for( auto c : setConstraints )
      c->cost( variableCost );

    std::cout << "Elapsed time: " << elapsedTime.count() << std::endl
	      << "Global cost: " << bestGlobalCost << std::endl
	      << "Grids:" << grid << std::endl;

    return bestGlobalCost;
  }
}
