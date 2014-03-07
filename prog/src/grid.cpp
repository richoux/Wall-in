#include "../include/grid.hpp"

namespace wallin
{
  Grid::Grid(int row, int col)
    : nRow_(row),
      mCol_(col),
      matrix_(std::vector< std::vector<std::string> >(row, std::vector<std::string>(col, "") ) )
  { }

  void Grid::add(int row, int col, Building& building)
  {
    for( int x = row; x < row + building.getHeight(); ++x )
      for( int y = col; y < col + building.getLength(); ++y )
	add(x, y, building.getShort() );
  }

  void Grid::add(int row, int col, std::string building)
  {
    bool fail = !matrix_[row][col].empty();
    matrix_[row][col] += building;
    if( fail )
    {
      std::pair<int, int> key(row, col);
      if( failures_.find( key ) == failures_.end() )
	failures_.emplace( key, matrix_[row][col] );
      else
	failures_.at( key ) += building;
    }
  }
  
  void Grid::clear(int row, int col, std::string building)
  {
    matrix_[row][col].replace( matrix_[row][col].find( building ),
			       building.length(),
			       "" );

    std::pair<int, int> key(row, col);
    mapFail::iterator it = failures_.find( key );

    if( it != failures_.end() )
    {
      if( matrix_[row][col].empty() )
	failures_.erase( it );
      else
	failures_.at( key ) = matrix_[row][col];
    }
  }

  std::ostream& operator<<( std::ostream& os, const Grid& g )
  {
    os << "#row: " <<  g.nRow_ << std::endl
       << "#column: " <<  g.mCol_ << std::endl
       << "Matrix:" << std::endl;

    std::string bar = "";
    for( int i=0; i<g.matrix_[0].size(); ++i )
      bar += "------";

    for( auto vec : g.matrix_ )
    {
      os << bar << std::endl << "| ";
      for(auto str : vec )
	os << std::setw(3) << (str.empty() ? " " : str) << " | ";

      os << std::endl;
    }
    os << bar << std::endl;
    
    os << "Failures:" << std::endl;
    for( auto m : g.failures_ )
      os << "(" << m.first.first << "," << m.first.second << "):" << m.second << std::endl;

    return os  << std::endl;
  }

}
