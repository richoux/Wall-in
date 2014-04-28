#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <cstdlib>

#include "building.hpp"
#include "random.hpp"

using namespace std;

namespace wallin
{
  using mapFail = map<pair<int, int>, string>;

  class Grid
  {
  public:
    Grid( int, int, int, int, int, int ) ;
    Grid( int, int, const vector< pair<int, int> >&, int, int, int, int ) ;
    Grid(const Grid&) = default;
    Grid(Grid&&) = default;
    Grid& operator=(const Grid&) = default;
    Grid& operator=(Grid&&) = default;
    ~Grid() = default;

	   void		  add  ( const Building& );
	   pair<int, int> shift( Building& );
	   void		  quickShift( Building& );
	   void		  clear( const Building& );
	   void		  swap( Building&, Building& );	  

    set< shared_ptr<Building> > getBuildingsAround ( const Building &, const vector< shared_ptr<Building> >& ) const;
    set< shared_ptr<Building> > getBuildingsAbove ( const Building &, const vector< shared_ptr<Building> >& ) const;
    set< shared_ptr<Building> > getBuildingsOnRight ( const Building &, const vector< shared_ptr<Building> >& ) const;
    set< shared_ptr<Building> > getBuildingsBelow ( const Building &, const vector< shared_ptr<Building> >& ) const;
    set< shared_ptr<Building> > getBuildingsOnLeft ( const Building &, const vector< shared_ptr<Building> >& ) const;
	   int	       	 countAround ( const Building &, const vector< shared_ptr<Building> >& ) const;  

	   int		 randomPos   ( const Building& );
           vector<int>	 possiblePos ( const Building& ) const;
           int		 distanceTo  ( int, std::pair<int, int> ) const;
    inline int		 distanceTo  ( int source, int target ) const { return distanceTo( source, lin2mat( target ) ); }
    inline int		 distanceToTarget( int source ) const { return distanceTo( source, targetTile ); }
	   void		 unbuildable ( vector< pair<int, int> > );
    inline void		 unbuildable ( int row, int col ) { matrixType_[row][col].assign(3, '#'); }

    inline set<int>	 buildingsAt( int row, int col ) const { return matrixId_[row][col]; }
    inline set<int>	 buildingsAt( pair<int, int> p ) const { return buildingsAt(p.first, p.second); }
    inline set<int>	 buildingsAt( int p ) const { return buildingsAt( lin2mat( p ) ); }

    inline pair<int, int> getStartingTile()	const { return startingTile; }
    inline pair<int, int> getTargetTile()	const { return targetTile; }
           
           bool		 isStartingOrTargetTile( int ) const;
    
    inline int		 getNberRows()	const { return nRow_; }
    inline int		 getNberCols()	const { return mCol_; }
    inline bool		 hasFailure()	const { return !failures_.empty(); }
    inline mapFail	 failures()	const { return failures_; }

    inline pair<int, int> lin2mat(int p)	    const {return make_pair(p / mCol_, p % mCol_);}
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
  };
}
