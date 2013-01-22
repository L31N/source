
#ifndef _NAVIGATION_CONTROLLER_H_
#define _NAVIGATION_CONTROLLER_H_

class NavigationController {
    public:
        NavigationController();
        ~NavigationController();

        int run();

    private:
        LaserSensor** laserSensors;
        MouseSensor** mouseSensors;
        CmpsSensor* cmps;

        Debug* debug;
        ipcReceivingConnection* ipcRCon;

        Vector* position;
        Vector* direction;
        Vector* speed;
};

#endif // _NAVIGATION_CONTROLLER_H_
