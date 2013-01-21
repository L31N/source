
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
        enum Direction {
            automatic = 0,
            clockwise = 1,
            anticlockwise = 2
        };

        ExtendedMotionController();
        ~ExtendedMotionController();

        void moveto(Vector vector, unsigned char speed = 255, Vector dir = Vector());
        void moveto(Vector vector, unsigned char speed = 255, Angle dir = Angle(0));

        void move(Vector vector, unsigned char speed = 255, Vector dir = Vector());
        void move(Vector vector, unsigned char speed = 255, Angle dir = Angle(0));

        void turnto(Vector dir, unsigned char speed = 255, Direction turndir = automatic);
        void turnto(Angle dir, unsigned char speed = 255, Direction turndir = automatic);

    private:
        //Navigation* navi;
};

#endif // _EXTENDED_MOTION_CONTROLLER_H_
