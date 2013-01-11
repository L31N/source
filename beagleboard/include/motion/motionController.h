
#ifndef _MOTION_CONTROLLER_H_
#define _MOTION_CONTROLLER_H_

#include "vector.hpp"
#include "angle.h"
#include "motor.h"

class MotionController {
    public:
        enum SpinDirection {
            autodir         = 0,
            clockwise       = 1,
            anticlockwise   = 2
        };

        MotionController();
        ~MotionController();

        void moveVector(Vector vector);
        void moveAngle(Angle angle);
        void turnAbsAngle(Angle angle, SpinDirection direction = autodir);
        void turnRelativeAngle(Angle angle, SpinDirection direction = autodir);

    private:
        Motor** motors;
        Debug* debug;
};

#endif // _MOTION_CONTROLLER_H_
