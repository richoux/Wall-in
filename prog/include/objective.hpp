#pragma once

#include <set>
#include <vector>
#include <memory>
#include <algorithm>

#include "building.hpp"
#include "grid.hpp"
#include "random.hpp"

namespace wallin
{
  class Objective
  {
  public:
    virtual double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const = 0;
    virtual int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) = 0;
    virtual int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& ) const;
  protected:
    Random randomVar;
  };

  class NoneObj : public Objective
  {
  public:
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
  };

  class GapObj : public Objective
  {
  public:
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
    //int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& ) const;    
  private:
    int gapSize( const std::shared_ptr<Building>, const std::vector< std::shared_ptr<Building> > &, const Grid& ) const;
  };

  class BuildingObj : public Objective
  {
  public:
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
  };

  class TechTreeObj : public Objective
  {
  public:
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
  };

  class FactoryObj
  {
  public:
    std::shared_ptr<Objective> makeObjective( const std::string& ) const;
  };
}
