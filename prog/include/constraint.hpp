#pragma once

#include <vector>
#include <memory>

#include "building.hpp"
#include "grid.hpp"

namespace wallin
{
  class Constraint
  {
  public:
    Constraint(std::vector< std::shared_ptr<Building> >, std::shared_ptr<Grid>) noexcept;
    virtual ~Constraint();

    virtual double cost() const = 0;
    
  protected:
    std::vector< std::shared_ptr<Building> > variables;
    std::shared_ptr<Grid> grid;
  };  

  //IsBuildable
  class IsBuildable : public Constraint
  {
  public:
    IsBuildable(std::vector< std::shared_ptr<Building> >, std::shared_ptr<Grid>) noexcept;
    double cost() const;
  };
}
