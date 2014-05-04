#pragma once

#include <set>
#include <vector>
#include <memory>
#include <algorithm>
#include <limits>

#include "building.hpp"
#include "grid.hpp"
#include "random.hpp"

namespace wallin
{
  class Objective
  {
  public:
    Objective( std::string );
    virtual double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const = 0;
    virtual int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) = 0;
    virtual void setHelper( const Building&, const std::vector< std::shared_ptr<Building> >&, const Grid& );
    void initHelper( int );
    void resetHelper();
    int heuristicValue( const std::vector< double > &vecPositions, double&, int&, const Grid& ) const;
    inline  std::string getName() { return name; }
  protected:
    Random randomVar;
    std::string name;
    std::vector<int> heuristicValueHelper; 
  };

  class NoneObj : public Objective
  {
  public:
    NoneObj( std::string );
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
  };

  class GapObj : public Objective
  {
  public:
    GapObj( std::string );
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
    void setHelper( const Building&, const std::vector< std::shared_ptr<Building> >&, const Grid& );
  private:
    int gapSize( const Building&, const std::vector< std::shared_ptr<Building> >&, const Grid& ) const;
  };

  class BuildingObj : public Objective
  {
  public:
    BuildingObj( std::string );
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
  };

  class TechTreeObj : public Objective
  {
  public:
    TechTreeObj( std::string );
    double cost( const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& ) const;
    int heuristicVariable( const std::vector< int > &vecVariables, const std::vector< std::shared_ptr<Building> > &vecBuildings, const Grid& );
  };

  class FactoryObj
  {
  public:
    std::shared_ptr<Objective> makeObjective( const std::string& ) const;
  };
}
