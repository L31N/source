#ifndef _MOTION_CONTROLLER_H_
#define _MOTION_CONTROLLER_H_

#include "motor.h"

enum Direction{FRONT,
				FRONTRIGHT,
				RIGHT,
				BACKRIGHT,
				BACK,
				BACKLEFT,
				LEFT,
				FRONTLEFT};

class MotionController {
    public:
        MotionController();
        ~MotionController();

        void drive(Direction dir, unsigned short speed);

        void pbreak();

    private:
        Motors motor;
};

#endif // _MOTION_CONTROLLER_H_
