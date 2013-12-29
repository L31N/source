#include "Navigation.h"

Navigation::Navigation() {

    #warning Names must be set
    laserSensor[0] = new LaserSensor("", "LASER_SENSOR0", 0);
    laserSensor[1] = new LaserSensor("", "LASER_SENSOR1", 1);
    laserSensor[2] = new LaserSensor("", "LASER_SENSOR2", 2);
    laserSensor[3] = new LaserSensor("", "LASER_SENSOR3", 3);

    debug = new Debug("NAVIGATION");


}

Navigation::~Navigation() {

}

void Navigation::run() {

}
