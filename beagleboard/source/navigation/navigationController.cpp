
// navigationController.cpp

#include "navigationController.h"

NavigationController::NavigationController() {
    laserSensors = new LaserSensor* [4];
    mouseSensors = new MouseSensor* [2];

    laserSensors[0] = new LaserSensor("LASER_SENSOR_0", "LASER_SENSOR_0", front);
    laserSensors[1] = new LaserSensor("LASER_SENSOR_1", "LASER_SENSOR_1", right);
    laserSensors[2] = new LaserSensor("LASER_SENSOR_2", "LASER_SENSOR_2", back);
    laserSensors[3] = new LaserSensor("LASER_SENSOR_3", "LASER_SENSOR_3", left);

    mouseSensors[0] = new MouseSensor("MOUSE_SENSOR_0");
    mouseSensors[1] = new MouseSensor("MOUSE_SENSOR_1");

    cmps = new CmpsSensor("CMPS_SENSOR", "CMPS_SENSOR");

    debug = new Debug("NAVIGATION");

    ipcRCon = new ipcReceivingConnection("NAVIGATION", IPC_LOCAL);

    position = new Vector();
    direction = new Vector();
    speed = new Vector();
}

NavigationController::~NavigationController() {
    delete speed;
    delete direction;
    delete position;

    delete ipcRCon;

    delete debug;

    delete cmps;

    for (int i = 0; i < 2; i++) delete mouseSensors[i];
    for (int i = 0; i < 4; i++) delete laserSensors[i];

    delete mouseSensors;
    delete laserSensors;
}

int NavigationController::run() {

}
