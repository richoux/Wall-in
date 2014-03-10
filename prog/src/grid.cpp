#include "../include/grid.hpp"

namespace wallin
{
  Grid::Grid(int row, int col)
    : nRow_(row),
      mCol_(col),
      matrix_(vector< vector<string> >(nRow_, vector<string>(mCol_, "") ) )
  { }

  Grid::Grid( int row, int col, vector< pair<int, int> > unbuildables )
    : Grid(row, col)
  {
    for( auto u : unbuildables )
      matrix_[u.first][u.second].assign(3, '#');
  }

  void Grid::add( Building& building )
  {
    pair<int, int> pos = lin2mat( building.getPosition() );
    int row = pos.first;
    int col = pos.second;

    for( int x = row; x < row + building.getHeight(); ++x )
      for( int y = col; y < col + building.getLength(); ++y )
	add(x, y, building.getShort() );
  }

  void Grid::add( int row, int col, string building )
  {
    bool fail = !matrix_[row][col].empty();
    matrix_[row][col] += building;
    if( fail )
    {
      pair<int, int> key(row, col);
      if( failures_.find( key ) == failures_.end() )
	failures_.emplace( key, matrix_[row][col] );
      else
	failures_.at( key ) += building;
    }
  }
  
  void Grid::clear( Building& building )
  {
    pair<int, int> pos = lin2mat( building.getPosition() );
    int row = pos.first;
    int col = pos.second;

    for( int x = row; x < row + building.getHeight(); ++x )
      for( int y = col; y < col + building.getLength(); ++y )
	clear(x, y, building.getShort() );
  }

  void Grid::clear( int row, int col, string building )
  {
    matrix_[row][col].replace( matrix_[row][col].find( building ),
			       building.length(),
			       "" );

    pair<int, int> key(row, col);
    mapFail::iterator it = failures_.find( key );

    if( it != failures_.end() )
    {
      if( matrix_[row][col].size() < 2 )
	failures_.erase( it );
      else
	failures_.at( key ) = matrix_[row][col];
    }
  }

  void Grid::unbuildable( vector< pair<int, int> > unbuildables )
  {
    for( auto u : unbuildables )
      this->unbuildable( u.first, u.second );    
  }

  ostream& operator<<( ostream& os, const Grid& g )
  {
    os << "#rows: " <<  g.nRow_ << endl
       << "#columns: " <<  g.mCol_ << endl
       << "Matrix:" << endl;

    string bar = "";
    for( int i=0; i<g.matrix_[0].size(); ++i )
      bar += "------";

    for( auto vec : g.matrix_ )
    {
      os << bar << endl << "| ";
      for(auto str : vec )
	os << setw(3) << (str.empty() ? " " : str) << " | ";

      os << endl;
    }
    os << bar << endl;
    
    os << "Failures:" << endl;
    for( auto m : g.failures_ )
      os << "(" << m.first.first << "," << m.first.second << "):" << m.second << endl;

    return os;
  }

}
