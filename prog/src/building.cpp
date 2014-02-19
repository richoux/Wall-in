#include "../include/building.hpp"

Building::Building(int x, int y, int top, int right, int bottom, int left) 
  : length(x),
    height(y),
    gapTop(top), 
    gapRight(right), 
    gapBottom(bottom), 
    gapLeft(left) 
{}

Building::~Building() {}
