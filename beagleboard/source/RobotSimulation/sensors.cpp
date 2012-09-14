
#include "sensors.h"

/** Class SENSOR **/
Sensor::Sensor(Robot* _robot, Angle _angle) : angle(_angle) { robot = _robot; }
Sensor::~Sensor() { delete robot; }

/** Class BALL_SENSOR **/
BallSensor::BallSensor(Robot* _robot, Ball* _ball, Angle _angle) : Sensor(_robot, _angle) {
    ball = _ball;
}

BallSensor::~BallSensor() { delete ball; }

int BallSensor::getValue() {
    Point* ballPosition = new Point(ball->getPosition().getX(), ball->getPosition().getY());

    /// write code for sensor-value calculation dependend to ball position here ...
    if (ballPosition == 0) return -1;

    return 0;
}

Angle BallSensor::getAngle(void) {
    return angle;
}
