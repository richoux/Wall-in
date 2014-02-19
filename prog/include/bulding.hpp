#pragma once

class Building
{
public:
  Building(int, int, int, int, int, int) noexcept;
  virtual ~Building();

  // no need of copy ctor and assignment operator
  // get rid of the Rule of Three here!

  inline int length() const	{ return length; }
  inline int height() const	{ return height; }

  inline int gapTop() const	{ return gapTop; }
  inline int gapRight() const	{ return gapRight; }
  inline int gapBottom() const	{ return gapBottom; }
  inline int gapLeft() const	{ return gapLeft; }

private:
  int length;
  int height;

  int gapTop;
  int gapRight;
  int gapBottom;
  int gatLeft;
};

class Academy : public Building
{
public:
  Academy() : Building(3, 2, 0, 3, 7, 8) { }
};

class Barracks : public Building
{
public:
  Barracks() : Building(4, 3, 8, 7, 15, 16) { }
};

class Bunker : public Building
{
public:
  Bunker() : Building(3, 2, 8, 15, 15, 16) { }
};

class EngineeringBay : public Building
{
public:
  EngineeringBay() : Building(4, 3, 16, 15, 19, 16) { }
};

class MissileTurret : public Building
{
public:
  MissileTurret() : Building(2, 2, 0, 15, 15, 16) { }
};

class Factory : public Building
{
public:
  Factory() : Building(4, 3, 8, 7, 7, 8) { }
};

class SupplyDepot : public Building
{
public:
  SupplyDepot() : Building(3, 2, 10, 9, 5, 10) { }
};
