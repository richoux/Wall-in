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
    Constraint(const Constraint&) = default;
    Constraint(Constraint&&) = default;
    Constraint& operator=(const Constraint&) = default;
    Constraint& operator=(Constraint&&) = default;
    virtual ~Constraint();

    virtual double cost( std::vector<double>& ) const = 0;
    virtual double simulateCost( const Building&, const Building& );

    inline void update( const Grid& g ) { grid = g; }

    friend std::ostream& operator<<( std::ostream&, const Constraint& );
    
  protected:
    std::vector< std::shared_ptr<Building> > variables;
    Grid grid;
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
  private:
    std::map<int, std::shared_ptr<Building> > mapBuildings;
  };
}
