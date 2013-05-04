
#include <string>
#include <iostream>

#include "sensor/laserSensor.h"


int main () {
    std::cout << "laser-sensor-testing ..." << std::endl;

    LaserSensor laser0("LASER_SENSOR0", "WENGLORS0", 0);
    LaserSensor laser1("LASER_SENSOR1", "WENGLORS1", 1);

    while(true) {
        std::cout << "distance[0]: " << laser0.getDistance(LaserSensor::mm) << std::endl;
        //std::cout << "distance[1]: " << laser1.getDistance(LaserSensor::mm) << std::endl;
        std::cout << std::endl;
        //sleep(1);
        usleep(50 * 1000);
    }
}

/*int main () {
    std::cout << "reflex_switch testing ..." << std::endl;

    ReflexSwitch* rswitch = new ReflexSwitch("REFLEX_BUTTON0", "REFLEX_BUTTON0");

    std::cout << "after ctor" << std::endl;
    while(true) {
        std::cout << "status: " << rswitch->getStatus() << std::endl;
        sleep(1);
    }

    delete rswitch;
    return 0;
}*/

/*int main () {
	std::cout << "starting ..." << std::endl;
	LaserSensor* laser = new LaserSensor("LASER_SENSOR0", "LASER_SENSOR0", LaserSensor::back);

	while(true) {
		std::cout << "distance: " << laser->getDistance() << std::endl;
		sleep(1);
	}

	return 0;
}*/

//int main () {
    //std::cout << "starting programm " << std::endl;
    /*CmpsSensor* cmps = new CmpsSensor("CMPS", 0x60, "/dev/i2c-2");
    std::cout << "before getAngle()" << std::endl;
    std::cout << "cmps: " << cmps->getAngle() << std::endl;*/

    //IRSensor* bsensor = new IRSensor("BALL_SENSOR0", "BALL_SENSOR0", IRSensor::G0);
    //sleep(1);
    //while(true) {
        //unsigned int status = bsensor->getStatus();
        //std::cout << "status: " << status << std::endl;
        //sleep(1);
    //}
    //delete cmps;
    //return 0;
//}
