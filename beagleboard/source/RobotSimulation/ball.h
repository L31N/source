
#ifndef _BALL_H_
#define _BALL_H_

#include "point.h"

class Ball {
    public:
        Ball(Point _position);
        ~Ball();

        Point getPosition(void);
        void setPosition(Point);

    private:
        Point position;
};

#endif //_BALL_H_
