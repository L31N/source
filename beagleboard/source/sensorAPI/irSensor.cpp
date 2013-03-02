
// irSensor.cpp

#include "irSensor.h"

IRSensor::IRSensor(const std::string ipcName, const std::string canMember) : CANSensor(ipcName, canMember) {
}

IRSensor::~IRSensor() {}

bool IRSensor::getStatus(unsigned char num) {
    if (num >= 24) {
        std::cerr << "IRSensor::getStatus --> invalid num: " << num << std::endl;
        debug->send("IRSensor::getStatus --> invalid num: %d", num);
        return false;
    }

    if (can->checkForNewData()) {
        unsigned char* tmp = can->getValue();

        values = 0;
        values |= ((unsigned int)*tmp << 2*8);
        values |= ((unsigned int)*(tmp+1) << 8);
        values |= ((unsigned int)*(tmp+2) << 0);
    }
    return ((values >> num) & 0x01);
}

unsigned char getBallNum(unsigned char num_of_sensors) {
    if (can->checkForNewData()) {
        unsigned char* tmp = can->getValue();
        values = 0;
        values |= ((unsigned int)*tmp << 2*8);
        values |= ((unsigned int)*(tmp+1) << 8);
        values |= ((unsigned int)*(tmp+2) << 0);
    }

    unsigned int tmp = 0;
    unsigned int maxConnected = 0;
    unsigned short max_angle_num = 0;

    for (int i = 0; i < num_of_sensors; i++) {
        if(this->getStatus(i)) {
            tmp ++;
            if (i == num_of_sensors - 1) {      // check for Zero-Intersection
                int j = 0;
                for (; this->getStatus(i); j++) tmp++;
                if (tmp > maxConnected) {
                    maxConnected = tmp;
                    max_angle_num = i + j;
                }
            }
            else {
                if (tmp > maxConnected) {
                    maxConnected = tmp;
                    max_angle_num = i;
                }
                tmp = 0;
            }
        }
    }

<<<<<<< HEAD
    maxAngleNum = (maxAngleNum + 1 - maxConnected + maxConnected/2) % (num_of_sensors + 1);     // calculates the central angle of the "high-section".
    return maxAngleNum;
=======
    max_angle_num = (max_angle_num + 1 - maxConnected + maxConnected/2) % (num_of_sensors + 1);     // calculates the central angle of the "high-section".
    return max_angle_num;
>>>>>>> 83a40c8bec75fadced99ac52c7410df8e6864320
}

