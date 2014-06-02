/*
 * Wall-in is a C++ library designed for StarCraft: Brood,
 * making a wall optimizised for a given objective: minimize the
 * number of buildings, the technology needed, the number of gaps
 * between building big enough to let enter small units, etc.
 * To do so, it use some Constraint Programming techniques 
 * like meta-heuristics.
 * Please visit https://github.com/richoux/Wall-in 
 * for further information.
 * 
 * Copyright (C) 2014 Florian Richoux
 *
 * This file is part of Wall-in.
 * Wall-in is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as published 
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Wall-in is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Wall-in. If not, see http://www.gnu.org/licenses/.
 */


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

  vector::set< Constraint* > makeProtossConstraints( const std::vector<std::shared_ptr<Building> >& vec, const Grid& grid )
  {
    overlap	 = make_shared<Overlap>( vec, grid );
    buildable	 = make_shared<Buildable>( vec, grid );
    noGaps	 = make_shared<NoGaps>( vec, grid );
    specialTiles = make_shared<StartingTargetTiles>( vec, grid );
    pylons	 = make_shared<Pylons>( vec, grid );
    
    vector< shared_ptr<Constraint> > vecConstraints {overlap, buildable, noGaps, specialTiles, pylons};
    return vecConstraints;
  }
}
