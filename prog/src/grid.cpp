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
  { 
    matrixType_[sRow][sCol] += "@s";
    matrixType_[tRow][tCol] += "@t";
  }

  Grid::Grid( int row, int col, const vector< pair<int, int> >& unbuildables, int sRow, int sCol, int tRow, int tCol ) 
    : Grid( row, col, sRow, sCol, tRow, tCol )
  {
    for( auto u : unbuildables )
      matrixType_[u.first][u.second].assign(3, '#');
  }

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

  vector<int> Grid::possiblePos( const Building& b ) const
  {
    vector<int> possiblePositions;

    possiblePositions.push_back( -1 );

    for( int row = 0; row <= nRow_ - b.getHeight(); ++row )
      for( int col = 0; col <= mCol_ - b.getLength(); ++col )
      {
	possiblePositions.push_back( mat2lin(row, col) );
      }

    return possiblePositions;
  }

  int Grid::distanceTo( int source, std::pair<int, int> target ) const
  {
    std::pair<int, int> sourcePair = lin2mat( source );
    return abs( target.first - sourcePair.first ) + abs( target.second - sourcePair.second );
  }

  void Grid::unbuildable( vector< pair<int, int> > unbuildables )
  {
    for( auto u : unbuildables )
      this->unbuildable( u.first, u.second );    
  }

  bool Grid::isStartingOrTargetTile( int id ) const
  {
    auto startingBuildings = buildingsAt( getStartingTile() );
    auto targetBuildings = buildingsAt( getTargetTile() );

    return startingBuildings.find( id ) != startingBuildings.end()
      || targetBuildings.find( id ) != targetBuildings.end();
  }

  ostream& operator<<( ostream& os, const Grid& g )
  {
    os << "#rows: " <<  g.nRow_ << endl
       << "#columns: " <<  g.mCol_ << endl
       << "Matrix Id:" << endl;

    string bar = "";
    for( int i=0; i<g.matrixType_[0].size(); ++i )
      bar += "------";

    for( auto vec : g.matrixId_ )
    {
      os << bar << endl << "| ";
      for( auto setId : vec )
      {
	if( setId.empty() )
	  os << setw(3) << "    | ";
	else
	{
	  for( auto id : setId )
	    os << setw(3) << to_string( id ) << " | ";
	}
      }
      os << endl;
    }
    os << bar << endl << endl;

    os << "Matrix Type:" << endl;
    for( auto vec : g.matrixType_ )
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
