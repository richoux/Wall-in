#include "../include/building.hpp"

namespace wallin
{

  Building::Building(int x, int y, int top, int right, int bottom, int left) 
    : length(x),
      height(y),
      gapTop(top), 
      gapRight(right), 
      gapBottom(bottom), 
      gapLeft(left),
      position(0)
  {}

  Building::Building(int x, int y, int top, int right, int bottom, int left, int position) 
    : Building(x, y, top, right, bottom, left),
      position(position)
  {}

  Building::~Building() {}

}
