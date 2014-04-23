#pragma once

#include <vector>
#include <memory>

#include "building.hpp"
#include "grid.hpp"
#include "random.hpp"

namespace wallin
{
  class Objective
  {
  public:
    virtual double cost( std::vector< std::shared_ptr<Building> > &vecBuildings ) = 0;
    virtual int heuristicVariable( const std::vector< int > &vecBuildings ) = 0;
    virtual int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& ) = 0;
  };

  class NoneObj : public Objective
  {
  public:
    double cost( std::vector< std::shared_ptr<Building> > &vecBuildings );
    int heuristicVariable( const std::vector< int > &vecVariables );
    int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& );
  private:
    Random randomVar;
  };

  class GapObj : public Objective
  {
  public:
    double cost( std::vector< std::shared_ptr<Building> > &vecBuildings );
    int heuristicVariable( const std::vector< int > &vecVariables );
    int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& );    
  };

  class BuildingObj : public Objective
  {
  public:
    double cost( std::vector< std::shared_ptr<Building> > &vecBuildings );
    int heuristicVariable( const std::vector< int > &vecVariables );
    int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& );    
  };

  class TechTreeObj : public Objective
  {
  public:
    double cost( std::vector< std::shared_ptr<Building> > &vecBuildings );
    int heuristicVariable( const std::vector< int > &vecVariables );
    int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& );    
  };

  class FactoryObj
  {
  public:
    std::shared_ptr<Objective> makeObjective( const std::string& );
  };
}
