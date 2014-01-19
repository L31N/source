#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <math.h>

#include "sensor/laserSensor.h"
#include "sensor/gyroSensor.h"
#include "debug/bbdebug.h"

#include "bbvector.h"
#include "bbline.h"

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

        Vector lastPositions[10];   // last position vectors

        Position positions[4];      // posible positions calculated with wenglors

        Vector vectors[4];          // last drive vectors

        ipcReceivingConnection* rcon;

        // private member functions
        unsigned char detectSituation(Vector pPos, Vector pDir);
        void calcPositions(unsigned char situation);

        void rankByPosition();
        void rankByVector();
        void rankByTrend();

        Position choosePosition(Position _positions[4]);

        static const unsigned long FIELD_WIDTH = 1810;
        static const unsigned long FIELD_HEIGHT = 2425;
};

#endif // NAVIGATION_H
