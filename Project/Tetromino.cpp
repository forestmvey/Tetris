// Author Forest Vey
#include "Tetromino.h"
#include <iostream>
#include <ctime>
using namespace std;

Tetromino::Tetromino() { // initialize a tetromino
	setShape(TetShape::SHAPE_S);
}
TetColor Tetromino::getColor() const // return tetromino colour
{
	return this->color;
}
TetShape Tetromino::getShape() // return tetromino shape
{
	return this->shape;
}

TetShape Tetromino::getRandomShape() {
	return(static_cast<TetShape>(rand() % 7));
}

void Tetromino::setShape(TetShape shape) // set a tetrominos shape
{
	blockLocs.empty();
	this->shape = shape;
	switch (shape) {
		case TetShape::SHAPE_S:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0,0),Point(-1,0), Point(0,1), Point(1,1) };
			break;
		case TetShape::SHAPE_Z:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0, 0), Point(1, 0), Point(0, 1), Point(-1, 1) };
			break;
		case TetShape::SHAPE_L:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0,0),Point(0,1), Point(0,-1), Point(1,-1) };
			break;
		case TetShape::SHAPE_J:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0,0),Point(0,1), Point(0,-1), Point(-1,-1) };
			break;
		case TetShape::SHAPE_O:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0,0),Point(0,1), Point(1,0), Point(1,1) };
			break;
		case TetShape::SHAPE_I:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0,0),Point(0,1), Point(0,2), Point(0,-1) };
			break;
		case TetShape::SHAPE_T:
			this->color = static_cast<TetColor>(static_cast<int>(shape));
			blockLocs = { Point(0,0),Point(1,0), Point(-1,0), Point(0,-1) };
			break;
	}
}
void Tetromino::rotateCW() {
	for (auto &point : this->blockLocs) {
		point.swapXY();
		point.multiplyY(-1);
	}
}
void Tetromino::printToConsole() const// print out a grid to represent coordinates of system
// start at top left [-3,-3] go to bottom right [3,3]
{
	bool spotted = false;
	for (int y = 3; y > -4; y--) {
		for (int x = -3; x < 4; x++) {
			for (auto const &point : this->blockLocs) {
				(point.getX() == x && point.getY() == y) ? spotted = true : spotted = false;
				if (spotted) break;
			}
			spotted ? cout << 'x' : cout << '.';
			spotted = false;
		}
		cout << endl;
	}
	
}