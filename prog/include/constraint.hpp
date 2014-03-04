#pragma once

#include <vector>
#include <memory>

#include "building.hpp"

namespace wallin
{

  using matrix = std::vector< std::vector< std::string > >;

  class Constraint
  {
  public:
    Constraint(std::vector< std::shared_ptr<Building> >, int, int);
    virtual ~Constraint();

    virtual double cost() const = 0;
    
  protected:
    std::vector< std::shared_ptr<Building> > variables;
    matrix grid;
  };  

  //IsBuildable
  class IsBuildable : public Constraint
  {
  public:
    IsBuildable(std::vector< std::shared_ptr<Building> >, int, int);
    double cost() const;
  };

}
