#include <iostream>
#include <typeinfo>

#include "../include/building.hpp"
#include "../include/constraint.hpp"
#include "../include/grid.hpp"

using namespace wallin;

void print(Building &b)
{
  std::cout 
    << "Type: " <<  typeid(b).name() << std::endl
    << "Position: " <<  b.getPosition() << std::endl
    << "Length: " <<  b.getLength() << std::endl
    << "Height: " <<  b.getHeight() << std::endl
    << "Gap Top: " <<  b.getGapTop() << std::endl
    << "Gap Right: " <<  b.getGapRight() << std::endl
    << "Gap Bottom: " <<  b.getGapBottom() << std::endl
    << "Gap Left: " <<  b.getGapLeft() << std::endl
    << "-------" << std::endl << std::endl;
}

int main(int argc, char **argv)
{
  Barracks b;
  Factory f;
  SupplyDepot s;

  b.setPos(9);
  print(b);
  print(f);
}
