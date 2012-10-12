
#include "ball.h"

Ball::Ball(Point _position) : position(_position) {}
Ball::~Ball() {}

Point Ball::getPosition(void) { return position; }
void Ball::setPosition(Point _position) { position = _position; }
