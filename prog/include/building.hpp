#pragma once

namespace wallin
{

  class Building
  {
  public:
    // ctor with linear position 
    Building(int, int, int, int, int, int, int = 0) noexcept;

    virtual ~Building() = 0;

    // no need of copy ctor and assignment operator
    // get rid of the Rule of Three here!

    inline void setPos(int pos)		{ position = pos; }
    inline int getPosition()	const	{ return position; }

    inline int getLength()	const	{ return length; }
    inline int getHeight()	const	{ return height; }

    inline int getGapTop()	const	{ return gapTop; }
    inline int getGapRight()	const	{ return gapRight; }
    inline int getGapBottom()	const	{ return gapBottom; }
    inline int getGapLeft()	const	{ return gapLeft; }

  protected:
    int position;

    int length;
    int height;

    int gapTop;
    int gapRight;
    int gapBottom;
    int gapLeft;
  };

  class Academy : public Building
  {
  public:
    Academy() : Building(3, 2, 0, 3, 7, 8) { }
    Academy(int pos) : Building(3, 2, 0, 3, 7, 8, pos) { }
  };

  class Barracks : public Building
  {
  public:
    Barracks() : Building(4, 3, 8, 7, 15, 16) { }
    Barracks(int pos) : Building(4, 3, 8, 7, 15, 16, pos) { }
  };

  class Bunker : public Building
  {
  public:
    Bunker() : Building(3, 2, 8, 15, 15, 16) { }
    Bunker(int pos) : Building(3, 2, 8, 15, 15, 16, pos) { }
  };

  class EngineeringBay : public Building
  {
  public:
    EngineeringBay() : Building(4, 3, 16, 15, 19, 16) { }
    EngineeringBay(int pos) : Building(4, 3, 16, 15, 19, 16, pos) { }
  };

  class MissileTurret : public Building
  {
  public:
    MissileTurret() : Building(2, 2, 0, 15, 15, 16) { }
    MissileTurret(int pos) : Building(2, 2, 0, 15, 15, 16, pos) { }
  };

  class Factory : public Building
  {
  public:
    Factory() : Building(4, 3, 8, 7, 7, 8) { }
    Factory(int pos) : Building(4, 3, 8, 7, 7, 8, pos) { }
  };

  class SupplyDepot : public Building
  {
  public:
    SupplyDepot() : Building(3, 2, 10, 9, 5, 10) { }
    SupplyDepot(int pos) : Building(3, 2, 10, 9, 5, 10, pos) { }
  };

} // namespace wallin
