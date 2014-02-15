
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <math.h>
#include <stdlib.h>

#include "sensor/laserSensor.h"
#include "sensor/gyroSensor.h"
#include "debug/bbdebug.h"

#include "bbvector.h"
#include "bbline.h"

#include "ipc/ipc_connection.h"

class Navigation
{
    public:
        Navigation();
        ~Navigation();

        void calculate();

        unsigned char getReliabilityX();
        unsigned char getReliabilityY();
        Vector getPosition();

    private:
        unsigned char reliabilityX;
        unsigned char reliabilityY;
        Vector position;

        LaserSensor* laserSensor[4];
        GyroSensor* gyro;

        Debug* debug;

        static const unsigned long FIELD_WIDTH = 1810;
        static const unsigned long FIELD_HEIGHT = 2425;

};

#endif
