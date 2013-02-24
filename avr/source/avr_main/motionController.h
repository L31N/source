
#ifndef _MOTION_CONTROLLER_H_
#define _MOTION_CONTROLLER_H_

#include "vector.h"
#include "motor.h"

class MotionController {
    public:
        MotionController();
        ~MotionController();

        void drive(Vector vector, short rotationSpeed = 0);
        //void drive(Angle angle, unsigned char speed, short rotationSpeed = 0);

        void pbreak();

    private:
        Motor motor;
        short* motorSpeeds;
};

#endif // _MOTION_CONTROLLER_H_
