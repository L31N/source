#include <iostream>

#include "sensor/laserSensor.h"


int main()
{
    LaserSensor *laser[4];

    laser[0] = new LaserSensor("LASER_SENSOR0", "WENGLORS0", 0);
    laser[1] = new LaserSensor("LASER_SENSOR1", "WENGLORS0", 1);
    laser[2] = new LaserSensor("LASER_SENSOR2", "WENGLORS1", 0);
    laser[3] = new LaserSensor("LASER_SENSOR3", "WENGLORS1", 1);

    while(true)
    {
        std::cout << laser[0]->getDistance() << "\t"  << laser[1]->getDistance() << "\t"  << laser[2]->getDistance() << "\t"  << laser[3]->getDistance() <<  std::endl;
        sleep(1);
    }

    return 0;
}
