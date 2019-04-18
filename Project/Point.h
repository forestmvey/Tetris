// Author Forest Vey
#ifndef POINT_H
#define POINT_H
#include <string>

class Point {
	public:
		Point();
		Point(int newX, int newY);
		int getX() const;
		int getY() const;
		void setX(int x);
		void setY(int y);
		void setXY(int x, int y);
		void swapXY();
		// multiply x by some factor
		void multiplyX(int factor);
		// multiply y by some factor
		void multiplyY(int factor);
		std::string toString() const;
		// return a string in the form of "[x,y]" to represent
		// the start of the point instance (for debugging)

	private:
		int x;
		int y;

		friend class TestSuite;
};

#endif