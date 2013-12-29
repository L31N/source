#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "sensor/laserSensor.h"
#include "debug/bbdebug.h"

class Navigation
{
    public:
        Navigation();
        ~Navigation();

        void run();

    private:
        LaserSensor *laserSensor[4];
        Debug *debug;

};

#endif // NAVIGATION_H
