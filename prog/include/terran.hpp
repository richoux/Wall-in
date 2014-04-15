#pragma once

#include <vector>
#include <memory>

#include "building.hpp"
#include "constraint.hpp"
#include "grid.hpp"

using namespace std;

namespace wallin
{

  shared_ptr<Building> a;
  shared_ptr<Building> b;
  shared_ptr<Building> u1;
  shared_ptr<Building> u2;
  shared_ptr<Building> e;
  shared_ptr<Building> f;
  shared_ptr<Building> t;
  shared_ptr<Building> s1;
  shared_ptr<Building> s2;

  shared_ptr<Constraint> overlap;
  shared_ptr<Constraint> buildable;
  shared_ptr<Constraint> noGaps;
  shared_ptr<Constraint> specialTiles;

  vector<shared_ptr<Building> > makeTerranBuildings()
  {
    a = make_shared<Academy>( );
    b = make_shared<Barracks>( );
    u1 = make_shared<Bunker>( );
    u2 = make_shared<Bunker>( );
    e = make_shared<EngineeringBay>( );
    f = make_shared<Factory>( );
    t = make_shared<MissileTurret>( );
    s1 = make_shared<SupplyDepot>( );
    s2 = make_shared<SupplyDepot>( );

    vector< shared_ptr<Building> > vec {a, b, u1, u2, e, f, t, s1, s2};
    return vec;
  }

  vector< shared_ptr<Constraint> > makeTerranConstraints( const vector<shared_ptr<Building> >& vec, const Grid& grid )
  {
    overlap	 = make_shared<Overlap>( vec, grid );
    buildable	 = make_shared<Buildable>( vec, grid );
    noGaps	 = make_shared<NoGaps>( vec, grid );
    specialTiles = make_shared<StartingTargetTiles>( vec, grid );
    
    vector< shared_ptr<Constraint> > setConstraints {overlap, buildable, noGaps, specialTiles};
    return setConstraints;
  }
}
