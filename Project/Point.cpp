// Author Forest Vey
#include "Point.h"
using namespace std;


Point::Point() {
	this->x = 0;
	this->y = 0;
};
Point::Point(int newX, int newY) {
	this->x = newX;
	this->y = newY;
};
int Point::getX() const {
	return this->x;
};
int Point::getY() const {
	return this->y;
};
void Point::setX(int x) {
	this->x = x;
};
void Point::setY(int y) {
	this->y = y;
};
void Point::setXY(int x, int y) {
	this->x = x;
	this->y = y;
};
void Point::swapXY() {
	int temp;
	temp = this->x;
	this->x = this->y;
	this->y = temp;
};
void Point::multiplyX(int factor) {
	this->x *= factor;
};
void Point::multiplyY(int factor) {
	this->y *= factor;
};
string Point::toString() const {
	return ("[" + to_string(x) + "," + to_string(y) + "]");
};

