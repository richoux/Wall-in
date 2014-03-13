#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <map>

#include "building.hpp"
#include "grid.hpp"

namespace wallin
{
  class Constraint
  {
  public:
    Constraint( const std::vector< std::shared_ptr<Building> >&, const Grid& ) noexcept;
    virtual ~Constraint();

    virtual double cost() const = 0;

    inline void update( const Grid& g ) { grid = g; }

    friend std::ostream& operator<<( std::ostream&, const Constraint& );
    
  protected:
    std::vector< std::shared_ptr<Building> > variables;
    Grid grid;
    std::map<int, std::shared_ptr<Building> > mapBuildings;
  };  

  //Overlap
  class Overlap : public Constraint
  {
  public:
    Overlap( const std::vector< std::shared_ptr<Building> >&, const Grid& ) noexcept;
    double cost() const;
  };

  //Buildable
  class Buildable : public Constraint
  {
  public:
    Buildable( const std::vector< std::shared_ptr<Building> >&, const Grid& ) noexcept;
    double cost() const;
  };

  //NoGaps
  class NoGaps : public Constraint
  {
  public:
    NoGaps( const std::vector< std::shared_ptr<Building> >&, const Grid& ) noexcept;
    double cost() const;
  };

  //StartingTargetTiles
  class StartingTargetTiles : public Constraint
  {
  public:
    StartingTargetTiles( const std::vector< std::shared_ptr<Building> >&, const Grid& ) noexcept;
    double cost() const;
  };
}
