
#ifndef _SENSORS_H_
#define _SENSORS_H_

#include "ball.h"
#include "angle.h"
#include "robot.h"

#include "vector.hpp"

/** deklaration of abstract base class SENSOR **/
class Sensor {
    public:
        Sensor(Robot* _robot, Angle _angle);
        ~Sensor();

        virtual int getValue() = 0;

    protected:
        Angle angle;
        Robot* robot;
};

class BallSensor : public Sensor {
    public:
        BallSensor(Robot* _robot, Ball* _ball, Angle _angle);
        ~BallSensor();

        int getValue(void);
        Angle getAngle(void);

    private:
        Ball* ball;
};

#endif //_SENSORS_H_
