#pragma once

#include <vector>
#include <memory>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

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

  std::unique_ptr<Overlap> overlap;
  std::unique_ptr<Buildable> buildable;
  std::unique_ptr<NoGaps> noGaps;
  std::unique_ptr<StartingTargetTiles> specialTiles;
  std::unique_ptr<Pylons> pylons;

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
    overlap.reset( new Overlap( vec, grid ) );
    buildable.reset( new Buildable( vec, grid ) );
    noGaps.reset( new NoGaps( vec, grid ) );
    specialTiles.reset( new StartingTargetTiles( vec, grid ) );
    pylons.reset( new Pylons( vec, grid ) );
    
    std::set< Constraint* > setConstraints;
    setConstraints.insert( overlap.get() );
    setConstraints.insert( buildable.get() );
    setConstraints.insert( noGaps.get() );  
    setConstraints.insert( specialTiles.get() );  
    setConstraints.insert( pylons.get() );  

    return setConstraints;
  }
}
