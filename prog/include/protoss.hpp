#pragma once

#include <vector>
#include <memory>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

using namespace std;

namespace wallin
{

  std::shared_ptr<Building> c;
  std::shared_ptr<Building> f;
  std::shared_ptr<Building> g1;
  std::shared_ptr<Building> g2;
  std::shared_ptr<Building> p1;
  std::shared_ptr<Building> p2;
  std::shared_ptr<Building> y1;
  std::shared_ptr<Building> y2;
  std::shared_ptr<Building> y3;
  std::shared_ptr<Building> y4;
  std::shared_ptr<Building> s;

  shared_ptr<Constraint> overlap;
  shared_ptr<Constraint> buildable;
  shared_ptr<Constraint> noGaps;
  shared_ptr<Constraint> specialTiles;
  shared_ptr<Constraint> pylons;

  std::vector<std::shared_ptr<Building> > makeProtossBuildings()
  {
    c = std::make_shared<CyberneticCore>( );
    f = std::make_shared<Forge>( );
    g1 = std::make_shared<Gateway>( );
    g2 = std::make_shared<Gateway>( );
    p1 = std::make_shared<PhotonCannon>( );
    p2 = std::make_shared<PhotonCannon>( );
    y1 = std::make_shared<Pylon>( );
    y2 = std::make_shared<Pylon>( );
    y3 = std::make_shared<Pylon>( );
    y4 = std::make_shared<Pylon>( );
    s = std::make_shared<ShieldBattery>( );

    std::vector<std::shared_ptr<Building> > vec {c, f, g1, g2, p1, p2, y1, y2, y3, y4, s};
    return vec;
  }

  std::set< Constraint* > makeProtossConstraints( const std::vector<std::shared_ptr<Building> >& vec, const Grid& grid )
  {
    overlap	 = make_shared<Overlap>( vec, grid );
    buildable	 = make_shared<Buildable>( vec, grid );
    noGaps	 = make_shared<NoGaps>( vec, grid );
    specialTiles = make_shared<StartingTargetTiles>( vec, grid );
    pylons	 = make_shared<Pylons>( vec, grid );
    
    set< shared_ptr<Constraint> > setConstraints {overlap, buildable, noGaps, specialTiles, pylons};
    return setConstraints;
  }
}
