#include "../include/solver.hpp"

#define TABU 8

namespace wallin
{
  Solver::Solver( const vector< shared_ptr<Constraint> >& vecConstraints, 
		  const vector<shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid,
		  const string &obj )
    : vecConstraints(vecConstraints), 
      vecBuildings(vecBuildings), 
      variableCost( vector<double>( vecBuildings.size(), 0. ) ),
      grid(grid),
      tabooList( vector<int>( vecBuildings.size(), 0 ) ),
      factory(FactoryObj()),
      objective(factory.makeObjective( obj ))
  { 
    reset();
  }

  void Solver::reset()
  {
    int xPos;
    int yPos;

    string shortName;
    bool supplyIncluded = false;

    for( auto b : vecBuildings )
    {
      shortName = b->getShort();

      // Start with one Barracks and one supply
      if( shortName.compare("B") == 0 || 
	  ( shortName.compare("S") == 0 && !supplyIncluded ) )
      {
	if( shortName.compare("S") == 0 )
	  supplyIncluded = true;

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
      
      for( auto n : neighbors )
	if( visited.find( n ) == visited.end() )
	  toVisit.insert( n );

      visited.insert( current );
    }

    return visited;
  }

  double Solver::solve( double timeout )
  {
    chrono::duration<double,milli> elapsedTime;
    chrono::time_point<chrono::system_clock> start;
    start = chrono::system_clock::now();

    // to time simulateCost and cost functions
    chrono::duration<double,milli> timeSimCost(0);
    chrono::duration<double,milli> timeCleaning(0);
    chrono::time_point<chrono::system_clock> startSimCost, startCleaning; 

#ifndef NDEBUG
    chrono::duration<double,milli> toverlap(0), tbuildable(0), tnogaps(0), tstt(0);
    chrono::time_point<chrono::system_clock> soverlap, sbuildable, snogaps, sstt; 
#endif
 
    int sizeGrid = grid.getNberRows() * grid.getNberCols() + 1; // + 1 for the "position -1" outside the grid
    vector< vector< double > > vecConstraintsCosts( vecConstraints.size(), vector<double>( vecBuildings.size(), 0. ) );
    vector< double >		 vecGlobalCosts( vector<double>( sizeGrid, -1 ) );
    vector< vector< double > > vecVarSimCosts( vector< vector<double> >( sizeGrid, vector<double>( vecBuildings.size(), 0. ) ) );

    double bestGlobalCost = numeric_limits<double>::max();
    double currentCost;
    double estimatedCost;
    double bestEstimatedCost = numeric_limits<double>::max();
    int    bestPosition;

    vector<int> worstBuildings;
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

	for( auto c : vecConstraints )
	  currentCost += c->cost( variableCost );

	if( currentCost < bestGlobalCost )
	  bestGlobalCost = currentCost;
	else
	{
	  reset();
	  continue;
	}
      }

      // make sure there is at least one untaboo variable
      bool freeVariables = false;

      // Update taboo list
      for( int i = 0; i < tabooList.size(); ++i )
      {
	if( tabooList[i] <= 1 )
	{
	  tabooList[i] = 0;
	  if( !freeVariables )
	    freeVariables = true;      
	}
	else
	  --tabooList[i];
      }

      // Here, we look at neighbor configurations with the lowest cost.
      worstBuildings.clear();
      worstVariableCost = 0;
      for( int i = 0; i < variableCost.size(); ++i )
      {
	if( !freeVariables || tabooList[i] == 0 )
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

      // look for the first smallest cost, according to objective heuristic
      bestSimCost = vecVarSimCosts[ objective->heuristicValue( vecGlobalCosts, bestEstimatedCost, bestPosition, grid) ];

      timeSimCost += chrono::system_clock::now() - startSimCost;

      currentCost = bestEstimatedCost;

      if( bestEstimatedCost < bestGlobalCost )
      {
	bestGlobalCost = bestEstimatedCost;
	variableCost = bestSimCost;
	move( oldBuilding, bestPosition );
      }
      else // local minima
	tabooList[ worstBuildingId ] = TABU;

      elapsedTime = chrono::system_clock::now() - start;
    } while( bestGlobalCost != 0. && elapsedTime.count() < timeout );

    // remove useless buildings
    if( bestGlobalCost == 0 )
    {
      startCleaning = chrono::system_clock::now();

      bool change;
      double cost;
      NoGaps ng( vecBuildings, grid );

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

      timeCleaning = chrono::system_clock::now() - startCleaning;
    }

    cout << "Grids:" << grid << endl
	 << "Elapsed time: " << elapsedTime.count() << endl
	 << "Global cost: " << bestGlobalCost << endl;

#ifndef NDEBUG
    cout << endl << "Elapsed time to simulate cost: " << timeSimCost.count() << endl
	 << "Elapsed time to clean: " << timeCleaning.count() << endl
	 << "Overlap: " << toverlap.count() << endl
	 << "Buildable: " << tbuildable.count() << endl
	 << "NoGaps: " << tnogaps.count() << endl
	 << "STT: " << tstt.count() << endl;


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

    return bestGlobalCost;
  }
}
