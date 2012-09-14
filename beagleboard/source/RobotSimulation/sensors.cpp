
#include "sensors.h"

#include <iostream>

/** Class SENSOR **/
Sensor::Sensor(Robot* _robot, Angle _angle) : angle(_angle) { robot = _robot; }
Sensor::~Sensor() { delete robot; }

/** Class BALL_SENSOR **/
BallSensor::BallSensor(Robot* _robot, Ball* _ball, Angle _angle) : Sensor(_robot, _angle) {
    ball = _ball;
}

BallSensor::~BallSensor() { delete ball; }

int BallSensor::getValue() {

    /// write code for sensor-value calculation dependend to ball position here ...
    Vector botOVector(robot->getPosition().getX(), robot->getPosition().getY());
    Vector ballOVector(ball->getPosition().getX(), ball->getPosition().getY());
    Vector toBallVector = ballOVector - botOVector;

    Angle absoluteBallAngle(toBallVector.getAngle());

    /// add frontAngle from Bot to angleToBall
    Angle relativeBallAngle((360 - robot->getFrontAngle().getValue() + absoluteBallAngle.getValue())%360);
    int distanceToBall = toBallVector.abs();

    int angleDifference;
    int abs_diff = abs( angle.getValue()-relativeBallAngle.getValue());
    bool turnover = bool( abs_diff/180);
    if(turnover)//Differenz größer gleich 180 -> Drehung über Null
    {
        if( relativeBallAngle.getValue() > angle.getValue())angleDifference = (relativeBallAngle.getValue()-360)-angle.getValue();//Linksdrehung
        else angleDifference = (360-angle.getValue())+relativeBallAngle.getValue();//Rechtsdrehung
    }
    else angleDifference = relativeBallAngle.getValue() - angle.getValue();//Normalfall
    if (angleDifference < 0) angleDifference *= -1;

    cout << "diffAngle: " << angleDifference << " ";

    /// calculate sensor intensity

    return (double(360*10) - double(angleDifference*10))/double(distanceToBall);
}

Angle BallSensor::getAngle(void) { return angle; }
