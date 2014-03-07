#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

#include "building.hpp"

using namespace std;

namespace wallin
{
  using mapFail = map<pair<int, int>, string>;

  class Grid
  {
  public:
    Grid(int, int);
    
    // inline string operator[](int i, int j) const { return matrix_[i][j]; }
    
    void add( Building& );
    void clear( Building& );

    inline bool		hasFailure()	const { return !failures_.empty(); }
    inline mapFail	failures()	const { return failures_; }

    inline int		mat2lin(int row, int col) const {return row * mCol_ + col;}
    inline int		mat2lin(pair<int, int> p) const {return p.first * mCol_ + p.second;}

    friend ostream& operator<<( ostream&, const Grid& );

  private:
    void add(int, int, string);
    void clear(int, int, string);
    
    inline pair<int, int> lin2mat(int p) const {return make_pair<int, int>(p / mCol_, p % mCol_);}

    int nRow_;
    int mCol_;
    vector< vector<string> > matrix_;
    mapFail failures_;
  };
}
