#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <map>

#include "building.hpp"
#include "grid.hpp"
#include "objective.hpp"

namespace wallin
{
  class Constraint
  {
  public:
    Constraint( const std::vector< std::shared_ptr<Building> >&, const Grid& );
    // Constraint(const Constraint&) = default;
    // Constraint(Constraint&&) = default;
    // Constraint& operator=(const Constraint&) = default;
    // Constraint& operator=(Constraint&&) = default;
    // virtual ~Constraint() = default;

    virtual double cost( std::vector<double>& ) const = 0;
    virtual std::vector<double> simulateCost( Building&, const std::vector<int>&, int, std::vector< std::vector<double> >&, std::shared_ptr<Objective>& );
    virtual std::vector<double> simulateCost( Building&, const std::vector<int>&, int, std::vector< std::vector<double> >& );
    virtual double simulateCost( Building&, const int, std::vector<double>& );

    inline void update( const Grid& g ) { grid = g; }

    friend std::ostream& operator<<( std::ostream&, const Constraint& );
    
  protected:
    bool isWall() const;
    
    std::vector< std::shared_ptr<Building> > variables;
    Grid grid;
  };  

  //Overlap
  class Overlap : public Constraint
  {
  public:
    Overlap( const std::vector< std::shared_ptr<Building> >&, const Grid& );
    double cost( std::vector<double>& ) const;
    std::vector<double> simulateCost( Building&, const std::vector<int>&, int, std::vector< std::vector<double> >& );
  };

  //Buildable
  class Buildable : public Constraint
  {
  public:
    Buildable( const std::vector< std::shared_ptr<Building> >&, const Grid& );
    double cost( std::vector<double>& ) const;
    std::vector<double> simulateCost( Building&, const std::vector<int>&, int, std::vector< std::vector<double> >& );
  };

  //NoGaps
  class NoGaps : public Constraint
  {
  public:
    NoGaps( const std::vector< std::shared_ptr<Building> >&, const Grid& );
    double cost( std::vector<double>& ) const;
  };

  //StartingTargetTiles
  class StartingTargetTiles : public Constraint
  {
  public:
    StartingTargetTiles( const std::vector< std::shared_ptr<Building> >&, const Grid& );
    double cost( std::vector<double>& ) const;
  private:
    std::map<int, std::shared_ptr<Building> > mapBuildings;
  };
}
