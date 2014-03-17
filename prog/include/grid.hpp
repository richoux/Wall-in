#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>

#include "building.hpp"
#include "tools.hpp"

using namespace std;

namespace wallin
{
  using mapFail = map<pair<int, int>, string>;

  class Grid
  {
  public:
    Grid( int, int, int, int, int, int ) noexcept;
    Grid( int, int, vector< pair<int, int> >, int, int, int, int ) noexcept;
    
	   void add  ( Building& );
	   void clear( Building& );
  
	   int	    countAround ( const Building &, const std::vector< std::shared_ptr<Building> >& ) const;
	   int	    randomPos   ( const Building& ) const ;
           set<int> possiblePos ( const Building& ) const;
	   void	    unbuildable ( vector< pair<int, int> > );
    inline void	    unbuildable ( int row, int col ) { matrixType_[row][col].assign(3, '#'); }

    inline set<int>	buildingsAt( int row, int col ) const { return matrixId_[row][col]; }
    inline set<int>	buildingsAt( pair<int, int> p ) const { return buildingsAt(p.first, p.second); }

    inline pair<int, int> getStartingTile()	const { return startingTile; }
    inline pair<int, int> getTargetTile()	const { return targetTile; }
    
    inline int		getNberRows()	const { return nRow_; }
    inline int		getNberCols()	const { return mCol_; }
    inline bool		hasFailure()	const { return !failures_.empty(); }
    inline mapFail	failures()	const { return failures_; }

    inline pair<int, int> lin2mat(int p)	    const {return make_pair<int, int>(p / mCol_, p % mCol_);}
    inline int		  mat2lin(int row, int col) const {return row * mCol_ + col;}
    inline int		  mat2lin(pair<int, int> p) const {return p.first * mCol_ + p.second;}

    friend ostream& operator<<( ostream&, const Grid& );

  private:
    void add(int, int, string, int);
    void clear(int, int, string, int);
    
    int nRow_;
    int mCol_;
    vector< vector<string> > matrixType_;
    vector< vector< set<int> > > matrixId_;
    pair<int, int> startingTile;
    pair<int, int> targetTile;
    mapFail failures_;

    Random randomValues;
  };
}
