#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <typeinfo>

#include "building.hpp"
#include "grid.hpp"

namespace wallin
{
  class Constraint
  {
  public:
    Constraint( std::vector< std::shared_ptr<Building> >&, Grid& ) noexcept;
    virtual ~Constraint();

    virtual double cost() const = 0;

    friend std::ostream& operator<<( std::ostream&, const Constraint& );
    
  protected:
    std::vector< std::shared_ptr<Building> > variables;
    Grid grid;
  };  

  //IsBuildable
  class IsBuildable : public Constraint
  {
  public:
    IsBuildable( std::vector< std::shared_ptr<Building> >&, Grid& ) noexcept;
    double cost() const;
  };
}
