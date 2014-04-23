#include "../include/objective.hpp"

namespace wallin
{

  /*************/
  /* Objective */
  /*************/
  int Objective::heuristicValue( const std::vector< double > &vecPositions, 
				 double &bestEstimatedCost,
				 int &bestPosition,
				 const Grid &grid ) const
  {
    int best = 0;

    for( int i = 0; i < vecPositions.size(); ++i )
    {
      if(      vecPositions[i] < bestEstimatedCost
	  || ( vecPositions[i] == bestEstimatedCost
	       && ( i == 0 || grid.distanceToTarget( i - 1 ) < grid.distanceToTarget( bestPosition ) ) ) )
      {
	bestEstimatedCost = vecPositions[i];
	bestPosition = i - 1;
	best = i;
      }
    }

    return best;
  }

  /***********/
  /* NoneObj */
  /***********/
  double NoneObj::cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid &grid ) const
  {
    return 0.;
  }

  int NoneObj::heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings )
  {
    return vecVariables[ randomVar.getRandNum( vecVariables.size() ) ];
  }

  /**********/
  /* GapObj */
  /**********/
  double GapObj::cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid &grid ) const
  {
    int gaps = 0;
    
    std::vector< std::shared_ptr<Building> > toVisit = vecBuildings;
    std::set< std::shared_ptr<Building> > neighbors;

    while( !toVisit.empty() )
    {
      auto b = *(toVisit.begin());
      neighbors = grid.getBuildingsAround( *b, toVisit );
      // for( auto n : neighbors )
      // 	if( )
    }
  }

  int GapObj::heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings )
  {

  }

  int GapObj::heuristicValue( const std::vector< double > &vecPositions,
			      double &bestEstimatedCost,
			      int &bestPosition,
			      const Grid &grid ) const
  {

  }

  /***************/
  /* BuildingObj */
  /***************/
  double BuildingObj::cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid &grid ) const
  {
    return std::count_if( vecBuildings.begin(), 
			  vecBuildings.end(), 
			  [](std::shared_ptr<Building> b){return b->isOnGrid();} );
  }

  int BuildingObj::heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings )
  {
    std::vector< int > varOnGrid( vecVariables.size() );
    
    auto it = std::copy_if( vecVariables.begin(),
    			    vecVariables.end(),
    			    varOnGrid.begin(),
    			    [&](int b){return vecBuildings[b]->isOnGrid();} );

    int size = std::distance( varOnGrid.begin(), it );

    if( it == varOnGrid.begin() )
    {
      varOnGrid = vecVariables;
      size = vecVariables.size();
    }

    return varOnGrid[ randomVar.getRandNum( size ) ];    
  }

  /***************/
  /* TechTreeObj */
  /***************/
  double TechTreeObj::cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid &grid ) const
  {
    auto max =  std::max_element( vecBuildings.begin(), 
				  vecBuildings.end(), 
				  [](std::shared_ptr<Building> b1, std::shared_ptr<Building> b2)
				  {return b1->getTreedepth() < b2->getTreedepth();} );

    return (*max)->getTreedepth();
  }

  int TechTreeObj::heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings )
  {
    auto min =  std::min_element( vecBuildings.begin(), 
				  vecBuildings.end(), 
				  [](std::shared_ptr<Building> b1, std::shared_ptr<Building> b2)
				  {return b1->getTreedepth() < b2->getTreedepth();} );

    int minValue = (*min)->getTreedepth();
    std::vector< int > varMinTech( vecVariables.size() );
    
    auto it = std::copy_if( vecVariables.begin(),
			    vecVariables.end(),
			    varMinTech.begin(),
			    [&](int b){return vecBuildings[b]->getTreedepth() == minValue;} );
    
    int size = std::distance( varMinTech.begin(), it );

    if( it == varMinTech.begin() )
    {
      varMinTech = vecVariables;
      size = vecVariables.size();
    }

    return varMinTech[ randomVar.getRandNum( size ) ];    
  }

  /**************/
  /* FactoryObj */
  /**************/
  std::shared_ptr<Objective> FactoryObj::makeObjective( const std::string &obj ) const
  {
    if( obj.compare("gap") == 0 || obj.compare("g") == 0 || obj.compare("G") == 0 )
      return std::make_shared<GapObj>();
    
    if( obj.compare("building") == 0 || obj.compare("b") == 0 || obj.compare("B") == 0 )
      return std::make_shared<BuildingObj>();
    
    if( obj.compare("techtree") == 0 || obj.compare("t") == 0 || obj.compare("T") == 0 )
      return std::make_shared<TechTreeObj>();
    
    return std::make_shared<NoneObj>();
  }
}
