#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

#include "building.hpp"

namespace wallin
{
  using mapFail = std::map<std::pair<int, int>, std::string>;

  class Grid
  {
  public:
    Grid(int, int);
    
    // inline std::string operator[](int i, int j) const { return matrix_[i][j]; }
    
    void add(int, int, Building&);

    inline bool		hasFailure()	const { return !failures_.empty(); }
    inline mapFail	failures()	const { return failures_; }

    friend std::ostream& operator<<( std::ostream&, const Grid& );

  private:
    void add(int, int, std::string);
    void clear(int, int, std::string);
    
    int nRow_;
    int mCol_;
    std::vector< std::vector<std::string> > matrix_;
    mapFail failures_;
  };
}
