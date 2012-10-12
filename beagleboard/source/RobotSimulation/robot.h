
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "point.h"
#include "angle.h"

class Robot {
    public:
        Robot(Point _position, Angle _frontAngle);
        ~Robot();

    private:
        Point position;
        Angle frontAngle;

    public:
        Angle getFrontAngle(void);
        Point getPosition(void);

        void setFrontAngle(Angle _frontAngle);
        void setPosition(Point _position);
};

#endif //_ROBOT_H_
