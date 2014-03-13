#pragma once

#include <vector>
#include <memory>

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

}
