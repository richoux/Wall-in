#include "../include/building.hpp"

namespace wallin
{
  int Building::nberBuildings = 0;

  Building::Building(int x, 
		     int y, 
		     int top, 
		     int right, 
		     int bottom, 
		     int left, 
		     std::string shortname, 
		     int position) noexcept
    : length(x),
      height(y),
      gapTop(top), 
      gapRight(right), 
      gapBottom(bottom), 
      gapLeft(left),
      shortname(shortname),
      position(position),
      id(Building::nberBuildings++)
  { }

  Building::~Building() {}

  std::ostream& operator<<( std::ostream& os, const Building& b )
  {
    return os
      << "Type: " <<  typeid(b).name() << std::endl
      << "Short name: " << b.shortname << std::endl
      << "Id num: " << b.id << std::endl
      << "Position: " <<  b.position << std::endl
      << "Length: " <<  b.length << std::endl
      << "Height: " <<  b.height << std::endl
      << "Gap Top: " <<  b.gapTop << std::endl
      << "Gap Right: " <<  b.gapRight << std::endl
      << "Gap Bottom: " <<  b.gapBottom << std::endl
      << "Gap Left: " <<  b.gapLeft << std::endl
      << "-------" << std::endl;
  }

}
