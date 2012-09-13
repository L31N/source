
#include "ball.h"
#include "robot.h"
#include "sensors.h"


const unsigned int NUM_OF_BALL_SENSORS = 30;

int main () {

    Ball ball(Point(10, 10));
    Robot robot(Point(0, 0), Angle(0));
    BallSensor* BallSensors[int(NUM_OF_BALL_SENSORS)];
    for (int i = 0; i < int(NUM_OF_BALL_SENSORS); i++) { BallSensors[i] = new BallSensor(&robot, &ball, i*(360/NUM_OF_BALL_SENSORS)); }


    /// Delete all Objects
    for(int i = 0; i < int(NUM_OF_BALL_SENSORS); i++) { delete BallSensors[i]; }
    return 0;
}
