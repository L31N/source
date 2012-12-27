
#include "robot.h"

Robot::Robot(Point _position, Angle _frontAngle) : position(_position), frontAngle(_frontAngle) {}
Robot::~Robot() {}

Angle Robot::getFrontAngle(void) { return frontAngle; }
Point Robot::getPosition(void) { return position; }

void Robot::setFrontAngle(Angle _frontAngle) { frontAngle = _frontAngle; }
void Robot::setPosition(Point _position) { position = _position; }
