
#ifndef _BALL_DETECTION_H_
#define _BALL_DETECTION_H_

#include <string>

#include "irSensor.h"

extern const unsigned int NUM_OF_SENSORS;
extern const unsigned int error_value_angle;

class BallDetection {
    public:
        BallDetection();
        ~BallDetection();

        unsigned int getBallAngle();

    private:
        IRSensor** ballSensors;
};

#endif // _BALL_DETECTION_H_
