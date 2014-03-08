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

    inline void update(Grid& g) { grid = g; }

    friend std::ostream& operator<<( std::ostream&, const Constraint& );
    
  protected:
    std::vector< std::shared_ptr<Building> > variables;
    Grid grid;
  };  

  //Overlap
  class Overlap : public Constraint
  {
  public:
    Overlap( std::vector< std::shared_ptr<Building> >&, Grid& ) noexcept;
    double cost() const;
  };
}
