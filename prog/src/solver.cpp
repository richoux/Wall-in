#include "../include/solver.hpp"

namespace wallin
{
  Solver::Solver( const std::set<Constraint*>& setConstraints, 
		  const std::vector<std::shared_ptr<Building> >& vecBuildings, 
		  const Grid& grid )
    : setConstraints(setConstraints), vecBuildings(vecBuildings), grid(grid)
  { }

  int Solver::solve( double timeout )
  {
    std::chrono::duration<double> elapsedTime;
    std::chrono::time_point<std::chrono::system_clock> start, now;
    start = std::chrono::system_clock::now();
    
    do {
      now = std::chrono::system_clock::now();
      elapsedTime = now - start;
      
      
    

    
    
    
    } while( elapsedTime.count() < timeout );
  }
}
