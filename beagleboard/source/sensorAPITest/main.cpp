
#include <string>
#include <iostream>

#include "bbvector.h"
#include "sensor/gyroSensor.h"
#include "sensor/laserSensor.h"
#include "sensor/gpio.h"
#include "sensor/irSensor.h"


int main () {

    IRSensor ir("BALL_SENSOR4", "BALL_SENSOR4", 4);

    while(true) {
        std::cout << "value: " << ir.getValue() << std::endl;
        usleep(100000);
    }


    GPIO gpio("USER_BUTTON0", 66, GPIO::OUTPUT);

    int value;
    while(true) {
        std::cin >> value;
        gpio.setValue(value);
    }


    while(true) {
        gpio.poll(-1);
        std::cout << "GPIO[30]: " << gpio.getValue() << std::endl;
    }

    std::cout << "laser-sensor-testing ..." << std::endl;

    /*LaserSensor* laser[4];
    laser[0] = new LaserSensor("LASER_SENSOR0", "WENGLORS0", 0);
    laser[1] = new LaserSensor("LASER_SENSOR1", "WENGLORS0", 1);
    laser[2] = new LaserSensor("LASER_SENSOR2", "WENGLORS1", 0);
    laser[3] = new LaserSensor("LASER_SENSOR3", "WENGLORS1", 1);

    while(true) {
        for (int i = 0; i < 4; i++) {
            std::cout << "LaserSensor" << i << ": [" << laser[i]->getDistance() << "]" << std::endl;
        }
        std::cout << "-------------------------------" << std::endl;
    }

    for (int i = 0; i < 4; i++) delete laser[i];

    return 0;*/

    GyroSensor gyro("GYRO");

    while(true) {

        Vector vect = gyro.getVector();
        vect.print();
        std::cout << vect.getAngle() << std::endl;

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
