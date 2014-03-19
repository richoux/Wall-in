#include "../include/grid.hpp"

namespace wallin
{
  Grid::Grid( int row, int col, int sRow, int sCol, int tRow, int tCol ) noexcept
    : nRow_(row),
      mCol_(col),
      matrixType_(vector< vector<string> >(nRow_, vector<string>(mCol_, "") ) ),
      matrixId_(vector< vector< set<int> > >(nRow_, vector< set<int> >(mCol_, set<int>() ) ) ),
      startingTile( make_pair( sRow, sCol ) ),
      targetTile( make_pair( tRow, tCol ) )
  { 
    matrixType_[sRow][sCol] += "@s";
    matrixType_[tRow][tCol] += "@t";
  }

  Grid::Grid( int row, int col, const vector< pair<int, int> >& unbuildables, int sRow, int sCol, int tRow, int tCol ) noexcept
    : Grid( row, col, sRow, sCol, tRow, tCol )
  {
    for( auto u : unbuildables )
      matrixType_[u.first][u.second].assign(3, '#');
  }

  // Grid::Grid( const Grid& other )
  //   : nRow_(other.nRow_),
  //     mCol_(other.mCol_),
  //     matrixType_(other.matrixType_),
  //     matrixId_(other.matrixId_),
  //     startingTile(other.startingTile),
  //     targetTile(other.targetTile),
  //     failures_(other.failures_)
  // { }

  // Grid& Grid::operator=(Grid other)
  // {
  //   std::swap(this->nRow_, other.nRow_);
  //   std::swap(this->mCol_, other.mCol_);
  //   std::swap(this->matrixType_, other.matrixType_);
  //   std::swap(this->matrixId_, other.matrixId_);
  //   std::swap(this->startingTile, other.startingTile);
  //   std::swap(this->targetTile, other.targetTile);
  //   std::swap(this->failures_, other.failures_);

  //   return *this;
  // }

  void Grid::add( const Building& building )
  {
    if( building.isOnGrid() )
    {
      pair<int, int> pos = lin2mat( building.getPosition() );
      int row = pos.first;
      int col = pos.second;
      
      for( int x = row; x < row + building.getHeight(); ++x )
	for( int y = col; y < col + building.getLength(); ++y )
	  add(x, y, building.getShort(), building.getId() );
    }
  }

  void Grid::add( int row, int col, string b_short, int b_id )
  {
    bool fail = ! ( matrixType_[row][col].empty() 
		    || ( matrixType_[row][col].find("@") != string::npos && matrixType_[row][col].size() <= 3) );

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
  
  void Grid::clear( const Building& building )
  {
    if( building.isOnGrid() )
    {
      pair<int, int> pos = lin2mat( building.getPosition() );
      int row = pos.first;
      int col = pos.second;
      
      for( int x = row; x < row + building.getHeight(); ++x )
	for( int y = col; y < col + building.getLength(); ++y )
	  clear(x, y, building.getShort(), building.getId() );
    }
  }

  // Not sure; fix later
  void Grid::clear( int row, int col, string b_short, int b_id )
  {
    auto it = matrixType_[row][col].find( b_short );
    if( it != std::string::npos )
    {
      matrixType_[row][col].replace( it,
				     b_short.length(),
				     "" );
      matrixId_[row][col].erase( b_id );
      
      pair<int, int> key(row, col);
      mapFail::iterator it = failures_.find( key );
      
      if( it != failures_.end() )
      {
	if( matrixType_[row][col].size() < 2 || matrixType_[row][col].compare("###") == 0 )
	  failures_.erase( it );
	else
	  failures_.at( key ) = matrixType_[row][col];
      }
    }
  }

  int Grid::countAround( const Building& b, const std::vector< std::shared_ptr<Building> >& variables ) const
  {
    if( b.isOnGrid() )
    {
      std::pair<int, int> coordinates = lin2mat( b.getPosition() );

      int top = coordinates.first;
      int right = coordinates.second + b.getLength() - 1;
      int bottom = coordinates.first + b.getHeight() - 1;
      int left = coordinates.second;

      int counter = 0;

      for(auto other : variables )
      {
	if( other->getId() != b.getId() && other->isOnGrid() )
	{
	  std::pair<int, int> xyOther = lin2mat( other->getPosition() );
	  int otherTop = xyOther.first;
	  int otherRight = xyOther.second + other->getLength() - 1;
	  int otherBottom = xyOther.first + other->getHeight() - 1;
	  int otherLeft = xyOther.second;

	  if(  ( top == otherBottom + 1 && ( otherRight >= left && otherLeft <= right ) )
	       || ( right == otherLeft - 1 && ( otherBottom >= top - 1 && otherTop <= bottom + 1 ) )
	       || ( bottom == otherTop - 1 && ( otherRight >= left && otherLeft <= right ) )
	       || ( left == otherRight + 1 && ( otherBottom >= top - 1 && otherTop <= bottom + 1 ) ) )
	  {
	    ++counter;
	  }
	}
      }

      return counter;
    }
    else
      return 0;
  }

  set<int> Grid::possiblePos( const Building& b ) const
  {
    set<int> possiblePositions;

    possiblePositions.insert( -1 );

    for( int row = 0; row <= nRow_ - b.getHeight(); ++row )
      for( int col = 0; col <= mCol_ - b.getLength(); ++col )
      {
	possiblePositions.insert( mat2lin(row, col) );
      }

    return possiblePositions;
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
