#include "../include/objective.hpp"

namespace wallin
{

  /***********/
  /* NoneObj */
  /***********/
  double NoneObj::cost( std::vector< std::shared_ptr<Building> > &vecBuildings )
  {
    return 0.;
  }

  int NoneObj::heuristicVariable( const std::vector< int > &vecVariables )
  {
    return vecVariables[ randomVar.getRandNum( vecVariables.size() ) ];
  }

  int NoneObj::heuristicValue( const std::vector< double > &vecPositions, 
			       double &bestEstimatedCost,
			       int &bestPosition,
			       const Grid &grid )
  {
    int best = 0;

    for( int i = 0; i < vecPositions.size(); ++i )
    {
      if(      vecPositions[i] < bestEstimatedCost
	  || ( vecPositions[i] == bestEstimatedCost
	       && ( i == 0 || grid.distanceToTarget( i - 1 ) < grid.distanceToTarget( bestPosition ) ) ) )
      {
	bestEstimatedCost = vecGlobalCosts[i];
	bestPosition = i - 1;
	best = i;
      }
    }

    return best;
  }

  /**********/
  /* GapObj */
  /**********/
  double GapObj::cost( std::vector< std::shared_ptr<Building> > &vecBuildings )
  {

  }

  int GapObj::heuristicVariable( const std::vector< int > &vecVariables )
  {

  }

  int GapObj::heuristicValue( const std::vector< double > &vecPositions,
			      double &bestEstimatedCost,
			      int &bestPosition,
			      const Grid &grid )
  {

  }

  /***************/
  /* BuildingObj */
  /***************/
  double BuildingObj::cost( std::vector< std::shared_ptr<Building> > &vecBuildings )
  {

  }

  int BuildingObj::heuristicVariable( const std::vector< int > &vecVariables )
  {

  }

  int BuildingObj::heuristicValue( const std::vector< double > &vecPositions,
				   double &bestEstimatedCost,
				   int &bestPosition,
				   const Grid &grid )
  {

  }

  /***************/
  /* TechTreeObj */
  /***************/
  double TechTreeObj::cost( std::vector< std::shared_ptr<Building> > &vecBuildings )
  {

  }

  int TechTreeObj::heuristicVariable( const std::vector< int > &vecVariables )
  {

  }

  int TechTreeObj::heuristicValue( const std::vector< double > &vecPositions, 
				   double &bestEstimatedCost,
				   int &bestPosition,
				   const Grid &grid )
  {

  }

  /**************/
  /* FactoryObj */
  /**************/
  std::shared_ptr<Objective> FactoryObj::makeObjective( const std::string &obj )
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
