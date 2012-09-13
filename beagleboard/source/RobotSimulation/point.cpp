
#include "point.h"

Point::Point(int _x, int _y) {
    x = _x;
    y = _y;
}

Point::~Point() {}

int Point::getX(void) { return x; }
int Point::getY(void) { return y; }

void Point::setX(int _x) { x = _x; }
void Point::setY(int _y) { y = _y; }

