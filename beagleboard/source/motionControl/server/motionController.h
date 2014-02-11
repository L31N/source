
#ifndef _MOTION_CONTROLLER_H_
#define _MOTION_CONTROLLER_H_

#include "bbvector.h"
//#include "angle.h"
#include "motor.h"
#include "debug/bbdebug.h"

class MotionController {
    public:
        MotionController();
        ~MotionController();

        void drive(Vector vector, short rotationSpeed = 0);
        //void drive(Angle angle, unsigned char speed, short rotationSpeed = 0);

        void pbreak();
        void idle();

    protected:
        Debug* debug;
        double getMotorSpeed(unsigned char num);

    private:
        Motors* motors;
        double* motorSpeeds;

        static const unsigned int MOTOR_MINIMUM_OFFSET = 35;
};

#endif // _MOTION_CONTROLLER_H_
