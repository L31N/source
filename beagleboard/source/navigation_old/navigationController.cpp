
// navigationController.cpp

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "navigationController.h"

NavigationController::NavigationController() {
    /*laserSensors = new LaserSensor* [4];
    mouseSensors = new MouseSensor* [2];

    laserSensors[0] = new LaserSensor("LASER_SENSOR_0", "LASER_SENSOR_0", LaserSensor::front);
    laserSensors[1] = new LaserSensor("LASER_SENSOR_1", "LASER_SENSOR_1", LaserSensor::right);
    laserSensors[2] = new LaserSensor("LASER_SENSOR_2", "LASER_SENSOR_2", LaserSensor::back);
    laserSensors[3] = new LaserSensor("LASER_SENSOR_3", "LASER_SENSOR_3", LaserSensor::left);

    mouseSensors[0] = new MouseSensor("MOUSE_SENSOR_0");
    mouseSensors[1] = new MouseSensor("MOUSE_SENSOR_1");

    cmps = new CmpsSensor("CMPS_SENSOR", "CMPS_SENSOR");

    debug = new Debug("NAVIGATION");

    ipcRCon = new ipcReceivingConnection("NAVIGATION", IPC_LOCAL);*/

    position = new Vector();
    direction = new Vector();
}

NavigationController::~NavigationController() {
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
    //#warning "NavigationController::run() not implemented yet"
    return 0;
}

Vector NavigationController::getPosition() {
    unsigned int x = 0;
    unsigned int y = 0;

    unsigned int laser_values[4];

    //for (int i = 0; i < 4; i++) {
        //laser_values[i] = laserSensors[i]->getDistance(LaserSensor::mm) + BOT_RAD;
    //}
    laser_values[0] = 89;
    laser_values[1] = 62;
    laser_values[2] = 96;
    laser_values[3] = 62;

    unsigned int d[2];
    unsigned int g[2];

    if (laser_values[0] + laser_values[2] <= laser_values[1] + laser_values[3]) {
        d[0] = laser_values[0];
        d[1] = laser_values[2];
        g[0] = laser_values[1];
        g[1] = laser_values[3];
    }
    else {
        d[0] = laser_values[1];
        d[1] = laser_values[3];
        g[0] = laser_values[0];
        g[1] = laser_values[2];
    }

    x = d[0]*cos(acos((MAX_X)/(d[0]+d[1])));
    y = g[0]*cos(acos((MAX_Y)/(g[0]+g[1])));

    return Vector(x, y);
}

Vector NavigationController::getDirection() {
    boost::asio::io_service io_service;
    io_service.run();

    Vector first = getPosition();

    boost::asio::deadline_timer timer(io_service, boost::posix_time::microseconds(10));
    timer.wait();

    Vector last = getPosition();

    io_service.stop();

    return Vector(first-last);


}

