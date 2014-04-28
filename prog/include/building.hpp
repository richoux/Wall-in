#pragma once

#include <algorithm>
#include <iostream>
#include <typeinfo>

using namespace std;

namespace wallin
{
  enum Race{ Terran, Protoss, Zerg, Unknown };
  
  class Building
  {
  public:
    Building(int, int, int, int, int, int, string, Race, int, int = -1);
    Building(const Building&) = default;
    Building(Building&&) = default;
    Building& operator=(const Building&) = default;
    Building& operator=(Building&&) = default;

    virtual ~Building() = 0;

    inline void setPos(int pos)		{ position = pos; }
    inline void shiftPos()		{ ++position; }
    inline int  getPosition()	const	{ return position; }
    inline bool isOnGrid()	const	{ return position != -1; }

    inline int    getId()	const	{ return id; }
    inline string getShort()	const	{ return shortname; }
    inline string getRace()	const	
      { 
	switch( race ) 
	{
	case Terran: return "Terran";
	case Protoss: return "Protoss";
	case Zerg: return "Zerg";
	default: return "Unknown";
	}
      }
    inline int getTreedepth()	const	{ return treedepth; }

    inline int getLength()	const	{ return length; }
    inline int getHeight()	const	{ return height; }
    inline int getSurface()	const	{ return height * length; }

    inline int getGapTop()	const	{ return gapTop; }
    inline int getGapRight()	const	{ return gapRight; }
    inline int getGapBottom()	const	{ return gapBottom; }
    inline int getGapLeft()	const	{ return gapLeft; }

    inline void swapPosition(Building &other)	{ std::swap(this->position, other.position); }

    inline bool operator<( const Building& other ) const { return id < other.id; }

    friend ostream& operator<<( ostream&, const Building& );

  protected:
    int length;
    int height;

    int gapTop;
    int gapRight;
    int gapBottom;
    int gapLeft;

    string shortname;
    int id;
    Race race;
    int treedepth;

    int position;
  private:
    static int nberBuildings;
  };

  class Academy : public Building
  {
  public:
    Academy() : Building(3, 2, 0, 3, 7, 8, "A", Terran, 2) { }
    Academy(int pos) : Building(3, 2, 0, 3, 7, 8, "A", Terran, 2, pos) { }
  };

  class Barracks : public Building
  {
  public:
    Barracks() : Building(4, 3, 8, 7, 15, 16, "B", Terran, 1) { }
    Barracks(int pos) : Building(4, 3, 8, 7, 15, 16, "B", Terran, 1, pos) { }
  };

  class Bunker : public Building
  {
  public:
    Bunker() : Building(3, 2, 8, 15, 15, 16, "U", Terran, 2) { }
    Bunker(int pos) : Building(3, 2, 8, 15, 15, 16, "U", Terran, 2, pos) { }
  };

  class EngineeringBay : public Building
  {
  public:
    EngineeringBay() : Building(4, 3, 16, 15, 19, 16, "E", Terran, 1) { }
    EngineeringBay(int pos) : Building(4, 3, 16, 15, 19, 16, "E", Terran, 1, pos) { }
  };

  class Factory : public Building
  {
  public:
    Factory() : Building(4, 3, 8, 7, 7, 8, "F", Terran, 2) { }
    Factory(int pos) : Building(4, 3, 8, 7, 7, 8, "F", Terran, 2, pos) { }
  };

  class MissileTurret : public Building
  {
  public:
    MissileTurret() : Building(2, 2, 0, 15, 15, 16, "T", Terran, 2) { }
    MissileTurret(int pos) : Building(2, 2, 0, 15, 15, 16, "T", Terran, 2, pos) { }
  };

  class SupplyDepot : public Building
  {
  public:
    SupplyDepot() : Building(3, 2, 10, 9, 5, 10, "S", Terran, 0) { }
    SupplyDepot(int pos) : Building(3, 2, 10, 9, 5, 10, "S", Terran, 0, pos) { }
  };

} // namespace wallin
