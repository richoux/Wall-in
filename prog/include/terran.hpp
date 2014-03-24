#pragma once

#include <vector>
#include <memory>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

namespace wallin
{

  std::shared_ptr<Building> a;
  std::shared_ptr<Building> b;
  std::shared_ptr<Building> u1;
  std::shared_ptr<Building> u2;
  std::shared_ptr<Building> e;
  std::shared_ptr<Building> f;
  std::shared_ptr<Building> t;
  std::shared_ptr<Building> s1;
  std::shared_ptr<Building> s2;

  std::unique_ptr<Overlap> overlap;
  std::unique_ptr<Buildable> buildable;
  std::unique_ptr<NoGaps> noGaps;
  std::unique_ptr<StartingTargetTiles> specialTiles;

  std::vector<std::shared_ptr<Building> > makeTerranBuildings()
  {
    a = std::make_shared<Academy>( );
    b = std::make_shared<Barracks>( );
    u1 = std::make_shared<Bunker>( );
    u2 = std::make_shared<Bunker>( );
    e = std::make_shared<EngineeringBay>( );
    f = std::make_shared<Factory>( );
    t = std::make_shared<MissileTurret>( );
    s1 = std::make_shared<SupplyDepot>( );
    s2 = std::make_shared<SupplyDepot>( );

    std::vector<std::shared_ptr<Building> > vec {a, b, u1, u2, e, f, t, s1, s2};
    return vec;
  }

  std::set< Constraint* > makeTerranConstraints( const std::vector<std::shared_ptr<Building> >& vec, const Grid& grid )
  {
    overlap.reset( new Overlap( vec, grid ) );
    buildable.reset( new Buildable( vec, grid ) );
    noGaps.reset( new NoGaps( vec, grid ) );
    specialTiles.reset( new StartingTargetTiles( vec, grid ) );
    
    std::set< Constraint* > setConstraints;
    setConstraints.insert( overlap.get() );
    setConstraints.insert( buildable.get() );
    setConstraints.insert( noGaps.get() );  
    setConstraints.insert( specialTiles.get() );  

    return setConstraints;
  }
}
