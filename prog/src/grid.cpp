#include "../include/grid.hpp"

namespace wallin
{
  Grid::Grid(int row, int col)
    : nRow_(row),
      mCol_(col),
      matrix_(std::vector< std::vector<std::string> >(row, std::vector<std::string>(col, "") ) )
  { }

  void Grid::add(int row, int col, std::string building)
  {
    bool fail = !matrix_[row][col].empty();
    matrix_[row][col] += building;
    if( fail )
    {
      std::pair<int, int> key(row, col);
      if( failures_.at( key ).empty() )
	failures_.emplace( key, matrix_[row][col] );
      else
	failures_.at( key ) += matrix_[row][col];
    }
  }
  
  void Grid::clear(int row, int col, std::string building)
  {
    
  }
}
