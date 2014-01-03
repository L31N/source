#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <math.h>

#include "sensor/laserSensor.h"
#include "sensor/gyroSensor.h"
#include "debug/bbdebug.h"
#include "bbvector.h"
#include "ipc/ipc_connection.h"

#include "position.h"

class Navigation
{
    public:
        Navigation();
        ~Navigation();

        void run();

    private:
        LaserSensor* laserSensor[4];
        GyroSensor* gyroSensor;
        Debug* debug;

        Vector lastPositions[10];

        Position* positions;

        Vector vectors[4];

        ipcReceivingConnection* rcon;

        // private member functions
        void rankByPosition(Position* position);
        void rankByVector(Position* position);
        void rankByTrend(Position* position);

        Position choosePosition(Position _positions[4]);
};

#endif // NAVIGATION_H
