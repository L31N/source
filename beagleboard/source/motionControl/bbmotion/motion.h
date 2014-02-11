
#ifndef _MOTION_H_
#define _MOTION_H_

#include <string>

#include "bbvector.h"
#include "ipc/ipc_connection.h"
#include "debug/bbdebug.h"

#include "angle.h"

class Motion {
    public:
        Motion();
        ~Motion();

        enum Direction {
            automatic,
            clockwise,
            anticlockwise
        };

        void drive(Vector vector, short rotationSpeed);

        void moveto(Vector vector, unsigned char speed, Vector dir);

        void move(Vector vector, unsigned char speed, Vector dir);

        void turnto(Vector dir, unsigned char speed, Motion::Direction turndir = Motion::automatic);

        void turn(Vector dir, unsigned char speed, Direction turndir = Motion::automatic);

        void pbreak();
        void idle();

        void test(unsigned char speed);

    private:
        Debug* dbg;
        ipcSendingConnection* scon;

};

#endif // _MOTION_H_
