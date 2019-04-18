// Author Forest Vey
#include "Gameboard.h"
#include <iostream>
#include <iomanip>
using namespace std;



Gameboard::Gameboard() {
	empty();
}								// constructor - empty() the grid

int Gameboard::getContent(Point pt) const				// return the content at a given point
{
	int x = pt.getX();
	int y = pt.getY();
	return getContent(x,y);
}
int Gameboard::getContent(int x, int y) const			// return the content at an x,y grid loc
{
	return grid[x][y];
}

void Gameboard::setContent(Point pt, int content)		// set the content at a given point
{
	int x = pt.getX();
	int y = pt.getY();
	grid[x][y] = content;
}
void Gameboard::setContent(int x, int y, int content)	// set the content at an x,y grid loc
{
	grid[x][y] = content;
}
void Gameboard::setContent(std::vector<Point> locs, int content)	// set the content for an array of grid locs
{
	for (auto &loc : locs) {
		int x = loc.getX();
		int y = loc.getY();
		setContent(x,y, content);
	}
}

bool Gameboard::areLocsEmpty(std::vector<Point> locs) const// return true if the content at all the locs specified is empty
											//   IMPORTANT NOTE: invalid x,y values can be passed to this method.
											//   Invalid meaning: outside the bounds of the grid.
											//   * Only test valid points (disregard the others - and ensure you
											//   don't use them to index into the grid).  Testing invalid points
											//   would likely result in an out of bounds error or segmentation fault!
{
	for (auto &loc : locs) {
		if (loc.getY() >= 0 && loc.getY() < MAX_Y && loc.getX() >= 0 && loc.getY() < MAX_Y) {
			if (getContent(loc) != EMPTY_BLOCK) return false;
		}
	}
	return true;
}
int Gameboard::removeCompletedRows()					// removes all completed rows from the board
											//   use getCompletedRowIndices() and removeRows() 
											//   return the # of completed rows removed
{
	vector<int> rowsToRemove = getCompletedRowIndices();
	removeRows(rowsToRemove);
	int removedRows = rowsToRemove.size();
	return removedRows;
}
void Gameboard::empty()								// fill the board with EMPTY_BLOCK 
											//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
{
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			grid[x][y] = EMPTY_BLOCK;
		}
	}
}
Point Gameboard::getSpawnLoc() const					// getter for the spawnLoc for new blocks
{
	return spawnLoc;
}
void Gameboard::printToConsole() const				// print the grid contents to the console (for debugging purposes)
											//   use setw(2) to space the contents out.
{
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			cout << setw(2) << grid[x][y];
		}
		cout << endl;
	}
}

bool Gameboard::isRowCompleted(int rowIndex) const	// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
{
	for (int x = 0; x < MAX_X; x++) {
		if (grid[x][rowIndex] == EMPTY_BLOCK) return false;
	}
	return true;
}
vector<int> Gameboard::getCompletedRowIndices() const	// scan the board for completed rows.
											//   Iterate through grid rows and use isRowCompleted(rowIndex)
											//   return a vector of completed row indices.
{
	vector<int> completedRows;
	for (int a = 0; a < MAX_Y; a++) {
		if (isRowCompleted(a)) {
			completedRows.push_back(a);
		}
	}
	return completedRows;
}
void Gameboard::removeRow(int rowIndex)				// In gameplay, when a full row is completed (filled with content)
											// it gets "removed".  To be exact, the row itself is not removed
											// but the content from the row above it is copied into it.
											// This continues all the way up the grid until the first row
											// is copied into the second row.  Finally, the first row is 
											// filled with EMPTY_BLOCK
											// given a row index:
											//   1) Starting at rowIndex, copy each row above the removed
											//     row "one-row-downwards" in the grid.
											//     (loop from y=rowIndex to 0, and copyRowIntoRow(y-1, y)).
											//   2) call fillRow() on the first row (and place EMPTY_BLOCKs in it).
{
	for (int y = rowIndex; y > 0; y--) {
		copyRowIntoRow(y - 1, y);
	}
	fillRow(0, EMPTY_BLOCK);
}
void Gameboard::removeRows(vector<int> rowIndices)// given a vector of row indices, remove them 
											//   (iterate through the vector and and call removeRow()
											//   on each row index).  
{
	for (auto &row : rowIndices) {
		removeRow(row);
	}
}
void Gameboard::fillRow(int rowIndex, int content)	// fill a given grid row with specified content
{
	for (int x = 0; x < MAX_X; x++) {
		grid[x][rowIndex] = content;
	}
}
void Gameboard::copyRowIntoRow(int sourceRowIndex,
	int targetRowIndex)	// copy a source row's contents into a target row.
{
	for (int x = 0; x < MAX_X; x++) {
		grid[x][targetRowIndex] = grid[x][sourceRowIndex];
	}
}



