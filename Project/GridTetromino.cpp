// Author Forest Vey
#include "GridTetromino.h"
using namespace std;


GridTetromino::GridTetromino() {// constructor, initialize gridLoc to 0,0
	gridLoc.setX(0);
	gridLoc.setY(0);
}			

Point GridTetromino::getGridLoc() const {// return the tetromino's grid/gameboard loc (x,y)
	return gridLoc;
}


void GridTetromino::setGridLoc(int x, int y) { // sets the tetromino's grid/gameboard loc using x,y
	gridLoc.setXY(x, y);
}	
void GridTetromino::setGridLoc(const Point& pt)		// sets the tetromino's grid/gameboard loc using a Point
{
	gridLoc.setXY(pt.getX(), pt.getY());
}

void GridTetromino::move(int xOffset, int yOffset) 
// transpose the gridLoc of this shape
//	(1,0) represents a move to the right (x+1)
//	(-1,0) represents a move to the left (x-1)
//	(0,1) represents a move down (y+1)
{
	gridLoc.setXY(xOffset + gridLoc.getX(), yOffset + gridLoc.getY());
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const 
// build and return a vector of Points to represent our inherited
// blockLocs vector mapped to the gridLoc of this object instance.
// eg: if we have a Point [x,y] in our vector,
// and our gridLoc is [5,6] the mapped Point would be [5+x,6+y].
{
	vector<Point>v;
	for (auto &point : blockLocs) {
		v.push_back(Point(gridLoc.getX() + point.getX(), gridLoc.getY() + point.getY()));
	}
	return v;
}
