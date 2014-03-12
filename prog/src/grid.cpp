#include "../include/grid.hpp"

namespace wallin
{
  Grid::Grid( int row, int col, int sRow, int sCol, int tRow, int tCol )
    : nRow_(row),
      mCol_(col),
      matrixType_(vector< vector<string> >(nRow_, vector<string>(mCol_, "") ) ),
      matrixId_(vector< vector< set<int> > >(nRow_, vector< set<int> >(mCol_, set<int>() ) ) ),
      startingTile( make_pair( sRow, sCol ) ),
      targetTile( make_pair( tRow, tCol ) )
  { }

  Grid::Grid( int row, int col, vector< pair<int, int> > unbuildables, int sRow, int sCol, int tRow, int tCol )
    : Grid( row, col, sRow, sCol, tRow, tCol )
  {
    for( auto u : unbuildables )
      matrixType_[u.first][u.second].assign(3, '#');
  }

  void Grid::add( Building& building )
  {
    pair<int, int> pos = lin2mat( building.getPosition() );
    int row = pos.first;
    int col = pos.second;

    for( int x = row; x < row + building.getHeight(); ++x )
      for( int y = col; y < col + building.getLength(); ++y )
	add(x, y, building.getShort(), building.getId() );
  }

  void Grid::add( int row, int col, string b_short, int b_id )
  {
    bool fail = !matrixType_[row][col].empty();
    matrixType_[row][col] += b_short;
    matrixId_[row][col].insert( b_id );
    if( fail )
    {
      pair<int, int> key(row, col);
      if( failures_.find( key ) == failures_.end() )
	failures_.emplace( key, matrixType_[row][col] );
      else
	failures_.at( key ) += b_short;
    }
  }
  
  void Grid::clear( Building& building )
  {
    pair<int, int> pos = lin2mat( building.getPosition() );
    int row = pos.first;
    int col = pos.second;

    for( int x = row; x < row + building.getHeight(); ++x )
      for( int y = col; y < col + building.getLength(); ++y )
	clear(x, y, building.getShort(), building.getId() );
  }

  void Grid::clear( int row, int col, string b_short, int b_id )
  {
    matrixType_[row][col].replace( matrixType_[row][col].find( b_short ),
			       b_short.length(),
			       "" );
    matrixId_[row][col].erase( b_id );

    pair<int, int> key(row, col);
    mapFail::iterator it = failures_.find( key );

    if( it != failures_.end() )
    {
      if( matrixType_[row][col].size() < 2 )
	failures_.erase( it );
      else
	failures_.at( key ) = matrixType_[row][col];
    }
  }

  void Grid::unbuildable( vector< pair<int, int> > unbuildables )
  {
    for( auto u : unbuildables )
      this->unbuildable( u.first, u.second );    
  }

  int Grid::countAround( const Building& b, const std::vector< std::shared_ptr<Building> >& variables ) const
  {
    std::pair<int, int> coordinates = lin2mat( b.getPosition() );

    int top = coordinates.first;
    int right = coordinates.second + b.getLength();
    int bottom = coordinates.first + b.getHeight();
    int left = coordinates.second;

    int counter = 0;

    for(auto other : variables )
    {
      if( other->getId() != b.getId() )
      {
	std::pair<int, int> xyOther = lin2mat( other->getPosition() );
	int otherTop = xyOther.first;
	int otherRight = xyOther.second + other->getLength();
	int otherBottom = xyOther.first + other->getHeight();
	int otherLeft = xyOther.second;
	
	if(  ( top == otherBottom + 1 && ( otherRight >= left && otherRight <= right + other->getLength() ) )
	  || ( right == otherLeft - 1 && ( otherBottom >= top - 1 && otherBottom <= bottom + 1 + other->getHeight() ) )
	  || ( bottom == otherTop - 1 && ( otherRight >= left && otherRight <= right + other->getLength() ) )
	  || ( left == otherRight + 1 && ( otherBottom >= top - 1 && otherBottom <= bottom + 1 + other->getHeight() ) ) )
	{
	  counter++;
	}
      }
    }

    return counter;
  }

  ostream& operator<<( ostream& os, const Grid& g )
  {
    os << "#rows: " <<  g.nRow_ << endl
       << "#columns: " <<  g.mCol_ << endl
       << "Matrix Type:" << endl;

    string bar = "";
    for( int i=0; i<g.matrixType_[0].size(); ++i )
      bar += "------";

    for( auto vec : g.matrixType_ )
    {
      os << bar << endl << "| ";
      for(auto str : vec )
	os << setw(3) << (str.empty() ? " " : str) << " | ";

      os << endl;
    }
    os << bar << endl << endl;

    os << "Matrix Id:" << endl;
    for( auto vec : g.matrixId_ )
    {
      os << bar << endl << "| ";
      for(auto setId : vec )
      {
	if( setId.empty() )
	  os << setw(3) << "    | ";
	else
	{
	  ostringstream oss;
	  for(auto id : setId)
	    oss << id;
	  os << setw(3) << oss.str() << " | ";
	}
      }
      os << endl;
    }
    os << bar << endl;
    
    os << "Failures:" << endl;
    for( auto m : g.failures_ )
      os << "(" << m.first.first << "," << m.first.second << "):" << m.second << endl;

    return os;
  }

}
