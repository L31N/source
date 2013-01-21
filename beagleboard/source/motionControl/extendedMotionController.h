
#ifndef _EXTENDED_MOTION_CONTROLLER_H_
#define _EXTENDED_MOTION_CONTROLLER_H_

#include "angle.h"
#include "motor.h"
#include "motionController.h"
#include "bbvector.h"
#include "bbdebug.h"
//#include "navigation.h"

class ExtendedMotionController : public MotionController {
    public:
        ExtendedMotionController();
        ~ExtendedMotionController();

        void moveto(Vector vector, Vector dir = Vector());
        void moveto(Vector vector, Angle dir = Angle(0));

        void move(Vector vector, Vector dir = Vector());
        void move(Vector vector, Angle dir = Angle(0));

        void turnto(Vector dir);
        void turnto(Angle dir);

    private:
        //Navigation* navi;
};

#endif // _EXTENDED_MOTION_CONTROLLER_H_
