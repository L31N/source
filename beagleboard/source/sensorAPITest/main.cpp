
#include <string>
#include <iostream>

#include "sensor/cmpsSensor.h"
#include "sensor/laserSensor.h"
#include "sensor/irSensor.h"
#include "sensor/reflexSwitch.h"
#include "sensor/button.h"

int main () {
    std::cout << "button testing ..." << std::endl;

    Button* button = new Button("USER_BUTTON0", "USER_BUTTON0");

    while(true) {
        std::cout << "BUTTON0 status: " << button->getStatus() << std::endl;
        sleep(1);
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
