
#ifndef _NAVIGATION_CONTROLLER_H_
#define _NAVIGATION_CONTROLLER_H_

#include <cmath>

#include "sensor/sensor.h"
#include "sensor/laserSensor.h"
#include "sensor/mouseSensor.h"
#include "sensor/cmpsSensor.h"

#include "ipc/bbipc.h"
#include "ipc/ipc_connection.h"

#include "bbcan.h"

#include "bbvector.h"
#include "bbdebug.h"

class NavigationController {
    public:
        NavigationController();
        ~NavigationController();

        int run();

        static const unsigned int MAX_X = 1820;
        static const unsigned int MAX_Y = 2430;

        static const unsigned int BOT_DIA = 220;
        static const unsigned int BOT_RAD = 110;

    private:
        LaserSensor** laserSensors;
        MouseSensor** mouseSensors;
        CmpsSensor* cmps;

        Debug* debug;
        ipcReceivingConnection* ipcRCon;

        Vector* position;
        Vector* direction;

        // ------------------- //
        Vector getPosition();
        Vector getDirection();

};

#endif // _NAVIGATION_CONTROLLER_H_
