
#include "ball.h"
#include "robot.h"
#include "sensors.h"

#include <iostream>

using namespace std;


const unsigned int NUM_OF_BALL_SENSORS = 30;

int main () {

    Ball ball(Point(-10, 0));
    Robot robot(Point(0, 0), Angle(0));
    BallSensor* ballSensors[int(NUM_OF_BALL_SENSORS)];
    for (int i = 0; i < int(NUM_OF_BALL_SENSORS); i++) { ballSensors[i] = new BallSensor(&robot, &ball, i*(360/NUM_OF_BALL_SENSORS)); }

    /** **/

    for (int i = 0; i < int(NUM_OF_BALL_SENSORS); i++) {
        cout << "Ball Position: " << ball.getPosition().getX() << "|" << ball.getPosition().getY() << " : Sensor[" << i << "] Angle: " << ballSensors[i]->getAngle().getValue() << " : Value: "<< ballSensors[i]->getValue() << endl;
    }

    /** **/

    /// Delete all Objects
    for(int i = 0; i < int(NUM_OF_BALL_SENSORS); i++) {
        delete ballSensors[i];
    }
    return 0;
}

