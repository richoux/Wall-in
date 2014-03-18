#pragma once

#include <iostream>
#include <typeinfo>

using namespace std;

namespace wallin
{

  class Building
  {
  public:
    // ctor with linear position 
    Building(int, int, int, int, int, int, string = "", int = 0) noexcept;
    Building(const Building&) = default;
    Building(Building&&) = default;
    Building& operator=(const Building&) = default;
    Building& operator=(Building&&) = default;

    virtual ~Building() = 0;

    inline void setPos(int pos)		{ position = pos; }
    inline int getPosition()	const	{ return position; }

    inline int    getId()	const	{ return id; }
    inline string getShort()	const	{ return shortname; }

    inline int getLength()	const	{ return length; }
    inline int getHeight()	const	{ return height; }

    inline int getGapTop()	const	{ return gapTop; }
    inline int getGapRight()	const	{ return gapRight; }
    inline int getGapBottom()	const	{ return gapBottom; }
    inline int getGapLeft()	const	{ return gapLeft; }

    friend ostream& operator<<( ostream&, const Building& );

  protected:
    int length;
    int height;

    int gapTop;
    int gapRight;
    int gapBottom;
    int gapLeft;

    string shortname;

    int position;
    int id;

  private:
    static int nberBuildings;
  };

  class Academy : public Building
  {
  public:
    Academy() : Building(3, 2, 0, 3, 7, 8, "A") { }
    Academy(int pos) : Building(3, 2, 0, 3, 7, 8, "A", pos) { }
  };

  class Barracks : public Building
  {
  public:
    Barracks() : Building(4, 3, 8, 7, 15, 16, "B") { }
    Barracks(int pos) : Building(4, 3, 8, 7, 15, 16, "B", pos) { }
  };

  class Bunker : public Building
  {
  public:
    Bunker() : Building(3, 2, 8, 15, 15, 16, "U") { }
    Bunker(int pos) : Building(3, 2, 8, 15, 15, 16, "U", pos) { }
  };

  class EngineeringBay : public Building
  {
  public:
    EngineeringBay() : Building(4, 3, 16, 15, 19, 16, "E") { }
    EngineeringBay(int pos) : Building(4, 3, 16, 15, 19, 16, "E", pos) { }
  };

  class Factory : public Building
  {
  public:
    Factory() : Building(4, 3, 8, 7, 7, 8, "F") { }
    Factory(int pos) : Building(4, 3, 8, 7, 7, 8, "F", pos) { }
  };

  class MissileTurret : public Building
  {
  public:
    MissileTurret() : Building(2, 2, 0, 15, 15, 16, "T") { }
    MissileTurret(int pos) : Building(2, 2, 0, 15, 15, 16, "T", pos) { }
  };

  class SupplyDepot : public Building
  {
  public:
    SupplyDepot() : Building(3, 2, 10, 9, 5, 10, "S") { }
    SupplyDepot(int pos) : Building(3, 2, 10, 9, 5, 10, "S", pos) { }
  };

} // namespace wallin
