#include "../include/solver.hpp"

constexpr int TABU	= 5;
constexpr int OPT_TIME	= 150;

namespace wallin
{
  Solver::Solver( const vector< shared_ptr<Constraint> >& vecConstraints, 
		  const vector<shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid,
		  const string &obj )
    : vecConstraints(vecConstraints), 
      vecBuildings(vecBuildings), 
      variableCost( vecBuildings.size() ),
      grid(grid),
      tabuList( vecBuildings.size() ),
      factory(FactoryObj()),
      objective(factory.makeObjective( obj )),
      bestSolution(vecBuildings.size())
  { 
    reset();
  }

  void Solver::reset()
  {
    int xPos;
    int yPos;

    string shortName;
    //bool supplyIncluded = false;

    clearAllInGrid( vecBuildings, grid );

    for( auto b : vecBuildings )
    {
      // 1 chance over 3 to be placed on the grid
      if( randomVar.getRandNum(3) == 0)
      {
	shortName = b->getShort();
	
	// Start with one Barracks and one supply
	// if( shortName.compare("B") == 0 || 
	//     ( shortName.compare("S") == 0 && !supplyIncluded ) )
	// {
	//   if( shortName.compare("S") == 0 )
	//     supplyIncluded = true;
	  
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

  set< shared_ptr<Building> > Solver::getNecessaryBuildings() const
  {
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

      visited.insert( current );
      
      for( auto n : neighbors )
	if( visited.find( n ) == visited.end() )
	  toVisit.insert( n );
    }

    return visited;
  }

  double Solver::solve( double timeout )
  {
    chrono::duration<double,milli> elapsedTime;
    chrono::duration<double,milli> elapsedTimeTour;
    chrono::duration<double,milli> postprocessGap(0);
    chrono::time_point<chrono::system_clock> start;
    chrono::time_point<chrono::system_clock> startTour;
    start = chrono::system_clock::now();

    // to time simulateCost and cost functions
    chrono::duration<double,milli> timeSimCost(0);
    chrono::time_point<chrono::system_clock> startSimCost; 

#ifndef NDEBUG
    chrono::duration<double,milli> toverlap(0), tbuildable(0), tnogaps(0), tstt(0);
    chrono::time_point<chrono::system_clock> soverlap, sbuildable, snogaps, sstt; 
#endif

    int sizeGrid = grid.getNberRows() * grid.getNberCols() + 1; // + 1 for the "position -1" outside the grid
    vector< vector< double > >  vecConstraintsCosts( vecConstraints.size() );
    vector< double >		vecGlobalCosts( sizeGrid );
    vector< vector< double > >  vecVarSimCosts( sizeGrid );

    bestCost = numeric_limits<double>::max();
    double bestGlobalCost = numeric_limits<double>::max();
    double globalCost;
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost;
    int    bestPosition;

    vector<int> worstBuildings;
    double worstVariableCost;
    int worstBuildingId;

    shared_ptr<Building> oldBuilding;
    vector<int> possiblePositions;
    vector<double> varSimCost( vecBuildings.size() );
    vector<double> bestSimCost( vecBuildings.size() );

    int tour = 0;

    do // optimization loop
    {
      startTour = chrono::system_clock::now();
      ++tour;
      globalCost = numeric_limits<double>::max();
      bestEstimatedCost = numeric_limits<double>::max();
      std::fill( varSimCost.begin(), varSimCost.end(), 0. );
      std::fill( bestSimCost.begin(), bestSimCost.end(), 0. );
      std::fill( vecConstraintsCosts.begin(), vecConstraintsCosts.end(), vector<double>( vecBuildings.size(), 0. ) );
      std::fill( vecVarSimCosts.begin(), vecVarSimCosts.end(), vector<double>( vecBuildings.size(), 0. ) );
      std::fill( variableCost.begin(), variableCost.end(), 0. );
      std::fill( tabuList.begin(), tabuList.end(), 0 );

      do // solving loop 
      {
	if( globalCost == numeric_limits<double>::max() )
	{
	  currentCost = 0.;

	  for( auto c : vecConstraints )
	    currentCost += c->cost( variableCost );

	  if( currentCost < globalCost )
	    globalCost = currentCost;
	  else
	  {
	    reset();
	    continue;
	  }
	}

	// make sure there is at least one untabu variable
	bool freeVariables = false;

	// Update tabu list
	for( int i = 0; i < tabuList.size(); ++i )
	{
	  if( tabuList[i] <= 1 )
	  {
	    tabuList[i] = 0;
	    if( !freeVariables )
	      freeVariables = true;      
	  }
	  else
	    --tabuList[i];
	}

	// Here, we look at neighbor configurations with the lowest cost.
	worstBuildings.clear();
	worstVariableCost = 0;
	for( int i = 0; i < variableCost.size(); ++i )
	{
	  if( !freeVariables || tabuList[i] == 0 )
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
      
	// can apply some heuristics here, according to the objective function
	worstBuildingId = objective->heuristicVariable( worstBuildings, vecBuildings, grid );
	oldBuilding = vecBuildings[ worstBuildingId ];
      
	// get possible positions for oldBuilding.
	possiblePositions = grid.possiblePos( *oldBuilding );

	// time simulateCost
	startSimCost = chrono::system_clock::now();

	// variable simulated costs
	fill( bestSimCost.begin(), bestSimCost.end(), 0. );

#ifndef NDEBUG
	soverlap = chrono::system_clock::now();
	vecConstraintsCosts[0] = vecConstraints[0]->simulateCost( *oldBuilding, possiblePositions, sizeGrid, vecVarSimCosts );
	toverlap += chrono::system_clock::now() - soverlap;

	sbuildable = chrono::system_clock::now();
	vecConstraintsCosts[1] = vecConstraints[1]->simulateCost( *oldBuilding, possiblePositions, sizeGrid, vecVarSimCosts );
	tbuildable += chrono::system_clock::now() - sbuildable;

	snogaps = chrono::system_clock::now();
	vecConstraintsCosts[2] = vecConstraints[2]->simulateCost( *oldBuilding, possiblePositions, sizeGrid, vecVarSimCosts );
	tnogaps += chrono::system_clock::now() - snogaps;

	sstt = chrono::system_clock::now();
	vecConstraintsCosts[3] = vecConstraints[3]->simulateCost( *oldBuilding, possiblePositions, sizeGrid, vecVarSimCosts );
	tstt += chrono::system_clock::now() - sstt;
#else
	for( int i = 0; i < vecConstraints.size(); ++i )
	  vecConstraintsCosts[i] = vecConstraints[i]->simulateCost( *oldBuilding, possiblePositions, sizeGrid, vecVarSimCosts );
#endif

	fill( vecGlobalCosts.begin(), vecGlobalCosts.end(), 0. );

	// sum all numbers in the vector vecConstraintsCosts[i] and put it into vecGlobalCosts[i] 
	for( auto v : vecConstraintsCosts )
	  transform( vecGlobalCosts.begin(), 
		     vecGlobalCosts.end(), 
		     v.begin(), 
		     vecGlobalCosts.begin(), 
		     plus<double>() );

	// replace all negative numbers by the max value for double
	replace_if( vecGlobalCosts.begin(), 
		    vecGlobalCosts.end(), 
		    bind( less<double>(), placeholders::_1, 0. ), 
		    numeric_limits<double>::max() );

	// for( int i = 0; i < vecConstraints.size(); ++i )
	// {
	//   cout << "vecConstraintsCosts[" << i << "] = ";
	//     for(auto d : vecConstraintsCosts[i])
	//       cout << d << " ";
	//   cout << endl;
	// }

	// cout << "vecGlobalCosts: ";
	// for(auto d : vecGlobalCosts)
	//   cout << d << " ";
	// cout << endl;
	

	// look for the first smallest cost, according to objective heuristic
	int b = objective->heuristicValue( vecGlobalCosts, bestEstimatedCost, bestPosition, grid);
	// cout << "Best i = " << b << endl;
	bestSimCost = vecVarSimCosts[ b ];

	timeSimCost += chrono::system_clock::now() - startSimCost;

	currentCost = bestEstimatedCost;

	// cout << "Best Sim Cost:";
	// for( auto i : bestSimCost )
	//   cout << i << " ";
	// cout << endl;

	// cout << "Current cost: " << currentCost << endl;

	if( bestEstimatedCost < globalCost )
	{
	  globalCost = bestEstimatedCost;

	  if( globalCost < bestGlobalCost )
	    bestGlobalCost = globalCost;

	  variableCost = bestSimCost;
	  move( oldBuilding, bestPosition );
	}
	else // local minima
	  tabuList[ worstBuildingId ] = TABU;

	elapsedTimeTour = chrono::system_clock::now() - startTour;
      } while( globalCost != 0. && elapsedTimeTour.count() < timeout );

      // remove useless buildings
      if( globalCost == 0 )
      {
	bool change;
	double cost;
	NoGaps ng( vecBuildings, grid );

	// cout << "BEFORE" << endl << grid;
	// printConstraints( vecConstraints );

	// remove all unreachable buildings from the starting building out of the grid
	set< shared_ptr<Building> > visited = getNecessaryBuildings();
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
	      
		cost = ng.simulateCost( *b, -1, varSimCost );
	      
		if( cost == 0. )
		{
		  grid.clear( *b );
		  b->setPos( -1 );
		  ng.update( grid );
		  change = true;
		}	  
	      }
	    }
	} while( change );

	double objectiveCost = objective->cost( vecBuildings, grid );
	if( objectiveCost < bestCost )
	{
	  // cout << "AFTER" << grid;
	  // cout << "global cost: " << globalCost << endl << "obj cost: " << objectiveCost << endl << endl;
	  // printConstraints( vecConstraints );
	  bestCost = objectiveCost;
	  for( int i = 0; i < vecBuildings.size(); ++i )
	    bestSolution[i] = vecBuildings[i]->getPosition();
	}      
      }
      reset();
      elapsedTime = chrono::system_clock::now() - start;
    }
    while( objective->getName().compare("none") != 0  && 
	   ( elapsedTime.count() < OPT_TIME || ( elapsedTime.count() >= OPT_TIME && bestGlobalCost != 0 ) ) );

    clearAllInGrid( vecBuildings, grid );

    for( int i = 0; i < vecBuildings.size(); ++i )
      vecBuildings[i]->setPos( bestSolution[i] );
    
    addAllInGrid( vecBuildings, grid );

    // For gap objective, try now to decrease the number of gaps.
    if( objective->getName().compare("gap") == 0 )
    {
      objective.reset( new GapObj("gap") );
      std::fill( tabuList.begin(), tabuList.end(), 0 );
        
      for( auto v : vecBuildings )
	buildingSameSize.insert( make_pair( v->getSurface(), v ) );

      vector<int> goodVar;
      shared_ptr<Building> toSwap;
      bool mustSwap;

      chrono::time_point<chrono::system_clock> startPostprocess = chrono::system_clock::now(); 
    
      bestCost = objective->cost( vecBuildings, grid );
      double currentCost = bestCost;
      cout << "bestCost: " << bestCost << endl;

      while( (postprocessGap = chrono::system_clock::now() - startPostprocess).count() < static_cast<int>( ceil(OPT_TIME / 100) ) && bestCost > 0 )
      {
	for( int i = 0; i < tabuList.size(); ++i )
	{
	  if( tabuList[i] <= 1 )
	    tabuList[i] = 0;
	  else
	    --tabuList[i];
	}

	for( int i = 0; i < vecBuildings.size(); ++i )
	{
	  if( tabuList[i] > 0 )
	    goodVar.push_back( i );
	}

	if( goodVar.empty() )
	  for( int i = 0; i < vecBuildings.size(); ++i )
	    goodVar.push_back( i );	

	int index = objective->heuristicVariable( goodVar, vecBuildings, grid );
	oldBuilding = vecBuildings[ index ];
	auto surface = buildingSameSize.equal_range( oldBuilding->getSurface() );
	
	for( auto it = surface.first; it != surface.second; ++it )
	{
	  mustSwap = false;
	  if( it->second->getId() != oldBuilding->getId() )
	  {
	    grid.swap( *it->second, *oldBuilding );
	    
	    currentCost = objective->cost( vecBuildings, grid );
	    if( currentCost < bestCost )
	    {
	      bestCost = currentCost;
	      toSwap = it->second;
	      mustSwap = true;
	      cout << "bestCost: " << bestCost << endl;
	    }

	    grid.swap( *it->second, *oldBuilding );
	  }
	  
	  if( mustSwap )
	    grid.swap( *toSwap, *oldBuilding );
	}

	tabuList[ index ] = std::max(2, static_cast<int>( ceil(TABU / 2) ) );
      }
    }
 
    cout << "Grids:" << grid << endl
	 << "Elapsed time: " << elapsedTime.count() << endl
	 << "Global cost: " << bestGlobalCost << endl
	 << "Optimization cost: " << bestCost << endl
	 << "Number of tours: " << tour << endl;

    if( objective->getName().compare("gap") == 0 )
      cout << "Post-processing time: " << postprocessGap.count() << endl; 

#ifndef NDEBUG
    cout << endl << "Elapsed time to simulate cost: " << timeSimCost.count() << endl
	 << "Overlap: " << toverlap.count() << endl
	 << "Buildable: " << tbuildable.count() << endl
	 << "NoGaps: " << tnogaps.count() << endl
	 << "STT: " << tstt.count() << endl;

    updateConstraints( vecConstraints, grid );

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
#endif

    return bestCost;
  }
}
