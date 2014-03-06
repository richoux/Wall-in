#include "../include/building.hpp"

namespace wallin
{

  Building::Building(int x, int y, int top, int right, int bottom, int left, int position) noexcept
    : length(x),
      height(y),
      gapTop(top), 
      gapRight(right), 
      gapBottom(bottom), 
      gapLeft(left),
      position(position)
  {}

  Building::~Building() {}

}
