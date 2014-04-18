#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const vector< shared_ptr<Constraint> >& vecConstraints, 
		  const vector<shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : vecConstraints(vecConstraints), 
      vecBuildings(vecBuildings), 
      variableCost( vector<double>( vecBuildings.size(), 0. ) ),
      grid(grid),
      tabooList( vector<int>( vecBuildings.size(), 0 ) )
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

  void Solver::move( shared_ptr<Building>& building, int newPosition )
  {
    grid.clear( *building );
    building->setPos( newPosition );
    grid.add( *building );
    updateConstraints( vecConstraints, grid );
  }

  double Solver::solve( double timeout )
  {
    chrono::duration<double,milli> elapsedTime;
    chrono::time_point<chrono::system_clock> start;
    start = chrono::system_clock::now();

    // to time simulateCost and cost functions
    chrono::duration<double,milli> timeSimCost(0);
    chrono::duration<double,milli> timeCost(0);
    chrono::duration<double,milli> timeCleaning(0);
    chrono::time_point<chrono::system_clock> startSimCost, startCost, startCleaning;
 
    // int sizeGrid = grid.getNberRows() * grid.getNberCols() + 1; // + 1 for the "position -1" outside the grid
    // vector< vector< double > > vecConstraintsCosts( vecConstraints.size(), vector<double>( vecBuildings.size(), 0. ) );
    // vector< double >		 vecGlobalCosts( vector<double>( sizeGrid, -1 ) );
    // vector< vector< double > > vecVarSimCosts( vector< vector<double> >( sizeGrid, vector<double>( vecBuildings.size(), 0. ) ) );

    double bestGlobalCost = numeric_limits<double>::max();
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost = numeric_limits<double>::max();
    int    bestPosition;

    vector<double> worstBuildings;
    double worstVariableCost;
    int worstBuildingId;

    shared_ptr<Building> oldBuilding;
    vector<int> possiblePositions;
    vector<double> varSimCost( vector<double>( vecBuildings.size(), 0. ) );
    vector<double> bestSimCost( vector<double>( vecBuildings.size(), 0. ) );

    int tour = 1;
    int test = 1;

    do 
    {
      if( bestGlobalCost == numeric_limits<double>::max() )
      {
	currentCost = 0.;

	// time costs
	//startCost = chrono::system_clock::now();
	
	for( auto c : vecConstraints )
	  currentCost += c->cost( variableCost );

	//timeCost = chrono::system_clock::now() - startCost;
	
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
      // fill( bestSimCost.begin(), bestSimCost.end(), 0. );

      // for( int i = 0; i < vecConstraints.size(); ++i )
      // 	vecConstraintsCosts[i] = vecConstraints[i]->simulateCost( *oldBuilding, possiblePositions, vecVarSimCosts );

      // fill( vecGlobalCosts.begin(), vecGlobalCosts.end(), -1 );
      
      // // sum all numbers in the vector vecConstraintsCosts[i] and put it into vecGlobalCosts[i] 
      // for( auto v : vecConstraintsCosts )
      // 	transform( vecGlobalCosts.begin(), 
      // 			vecGlobalCosts.end(), 
      // 			v.begin(), 
      // 			vecGlobalCosts.begin(), 
      // 			plus<double>() );
      
      // // replace all negative numbers by the max value for double
      // replace_if( vecGlobalCosts.begin(), 
      // 		       vecGlobalCosts.end(), 
      // 		       bind( less<double>(), placeholders::_1, 0. ), 
      // 		       numeric_limits<double>::max() );

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
      	fill( varSimCost.begin(), varSimCost.end(), 0. );
      
      	// time simulateCost
      	startSimCost = chrono::system_clock::now();

      	for( auto c : vecConstraints )
      	  estimatedCost += c->simulateCost( *oldBuilding, pos, varSimCost );

      	timeSimCost += chrono::system_clock::now() - startSimCost;

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

      elapsedTime = chrono::system_clock::now() - start;
    } while( bestGlobalCost != 0. && elapsedTime.count() < timeout );

    // remove useless buildings
    if( bestGlobalCost == 0 )
    {
      startCleaning = chrono::system_clock::now();

      bool change;
      double cost;
      NoGapsFinalize ngf( vecBuildings, grid );

      // find all buildings accessible from the starting building and remove all others
      int nberCurrent = *( grid.buildingsAt( grid.getStartingTile() ).begin() );
      shared_ptr<Building> current = vecBuildings[ nberCurrent ];
      set< shared_ptr<Building> > toVisit = grid.getBuildingsAround( *current, vecBuildings );
      set< shared_ptr<Building> > visited;
      set< shared_ptr<Building> > neighbors;

      visited.insert( current );

      while( !toVisit.empty() )
      {
	auto first = *( toVisit.begin() );
	current = first;
	toVisit.erase( first );
	neighbors = grid.getBuildingsAround( *current, vecBuildings );

	for( auto n : neighbors )
	  if( visited.find( n ) == visited.end() )
	    toVisit.insert( n );

	visited.insert( current );
      }

      // remove all unreachable buildings from the starting building out of the grid
      for( auto b : vecBuildings )
	if( visited.find( b ) == visited.end() )
	{
	  grid.clear( *b );
	  b->setPos( -1 );
	}

      // clean wall from unnecessary buildings.
      do
      {
    	for( auto b : vecBuildings )
	  if( ! grid.isStartingOrTargetTile( b->getId() ) )
	  {
	    change = false;
	    if( b->isOnGrid() )
	    {
	      cost = 0.;
	      fill( varSimCost.begin(), varSimCost.end(), 0. );
	      
	      cost = ngf.simulateCost( *b, -1, varSimCost );
	      
	      if( cost == 0. )
	      {
		grid.clear( *b );
		b->setPos( -1 );
		ngf.update( grid );
		change = true;
	      }	  
	    }
	  }
      } while( change );

      timeCleaning = chrono::system_clock::now() - startCleaning;
    }

    cout << "Grids:" << grid << endl
	 << "Elapsed time: " << elapsedTime.count() << endl
	 << "Elapsed time to simulate cost: " << timeSimCost.count() << endl
      //<< "Elapsed time to cost: " << timeCost.count() << endl
	 << "Elapsed time to clean: " << timeCleaning.count() << endl
	 << "Global cost: " << bestGlobalCost << endl;

    // print cost for each constraint
    for( auto c : vecConstraints )
    {
      fill( varSimCost.begin(), varSimCost.end(), 0. );
      cout << "Cost of " << typeid(*c).name() << ": " << c->cost( varSimCost ) << " [";

      for( auto v : varSimCost )
	cout << " " << v;

      cout << " ]" << endl;
    }      
    
    cout << endl;
    
    return bestGlobalCost;
  }
}
