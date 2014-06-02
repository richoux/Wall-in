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


#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
//#include <set>

#include <type_traits>

#include "../include/building.hpp"
#include "../include/constraint.hpp"
#include "../include/grid.hpp"
#include "../include/tools.hpp"
#include "../include/terran.hpp"
#include "../include/solver.hpp"

using namespace wallin;

int main(int argc, char **argv)
{
  std::vector< std::pair<int, int> > unbuildables 
  { 
    std::make_pair(7, 12), 
    std::make_pair(7, 13), 
    std::make_pair(7, 14), 
    std::make_pair(7, 15), 
    std::make_pair(8, 10), 
    std::make_pair(8, 11), 
    std::make_pair(8, 12), 
    std::make_pair(8, 13), 
    std::make_pair(8, 14), 
    std::make_pair(8, 15), 
    std::make_pair(9, 10), 
    std::make_pair(9, 11), 
    std::make_pair(9, 12), 
    std::make_pair(9, 13), 
    std::make_pair(9, 14), 
    std::make_pair(9, 15), 
    std::make_pair(10, 8), 
    std::make_pair(10, 9), 
    std::make_pair(10, 10), 
    std::make_pair(10, 11), 
    std::make_pair(10, 12), 
    std::make_pair(10, 13), 
    std::make_pair(10, 14), 
    std::make_pair(10, 15), 
    std::make_pair(11, 8), 
    std::make_pair(11, 9), 
    std::make_pair(11, 10), 
    std::make_pair(11, 11), 
    std::make_pair(11, 12), 
    std::make_pair(11, 13), 
    std::make_pair(11, 14), 
    std::make_pair(11, 15) 
  };
  
  Grid grid( 16, 12, unbuildables, 11, 7, 6, 15 );

  // Please write the name of the objective here!
  std::string objective = "g";

  //std::vector<std::shared_ptr<Building> > vec			= makeTerranBuildings( objective );
  std::vector<std::shared_ptr<Building> > vec			= makeTerranBuildings();
  std::vector< std::shared_ptr<Constraint> > vecConstraints	= makeTerranConstraints( vec, grid );

  Solver solver( vecConstraints, vec, grid, objective );

#ifndef NDEBUG
  std::cout << std::boolalpha << "Building movable: " << std::is_nothrow_move_constructible<Building>::value << std::endl;
  std::cout << std::boolalpha << "Barracks movable: " << std::is_nothrow_move_constructible<Barracks>::value << std::endl;
  std::cout << std::boolalpha << "Grid movable: " << std::is_nothrow_move_constructible<Grid>::value << std::endl;
  std::cout << std::boolalpha << "Solver movable: " << std::is_nothrow_move_constructible<Solver>::value << std::endl;
  std::cout << std::boolalpha << "Random movable: " << std::is_nothrow_move_constructible<Random>::value << std::endl;
  std::cout << std::boolalpha << "Constraint movable: " << std::is_nothrow_move_constructible<Constraint>::value << std::endl;
  std::cout << std::boolalpha << "Overlap movable: " << std::is_nothrow_move_constructible<Overlap>::value << std::endl;
  std::cout << std::boolalpha << "Objective movable: " << std::is_nothrow_move_constructible<Objective>::value << std::endl;
  std::cout << std::boolalpha << "GapObj movable: " << std::is_nothrow_move_constructible<GapObj>::value << std::endl;
#endif

  solver.solve( 20 );    
}
