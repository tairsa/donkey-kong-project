#include "Point.h"

void Point::setXY(int _x, int _y) {
	x = _x; 
	y = _y; 
	if (pBoard) {
		underlyingChar = pBoard->getChar(x, y);
	}
}


