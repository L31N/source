
// ballDetection.cpp

#include <sstream>
#include <limits>

#include "ballDetection.h"

const unsigned int error_value_angle = std::numeric_limits<unsigned int>::max();

const unsigned int NUM_OF_SENSORS = 16;

BallDetection::BallDetection() {
    ballSensors = new IRSensor*[NUM_OF_SENSORS];
    for (unsigned int i = 0; i < NUM_OF_SENSORS; i++) {
        std::string ipcName;
        std::string canMember;

        ipcName = "BallSensor";
        std::stringstream ss;
        ss << i;
        ss >> ipcName;

        canMember = ipcName;

        ballSensors[i] = new IRSensor(ipcName, canMember, IRSensor::Angle(i));
    }
}

BallDetection::~BallDetection() {
    delete *ballSensors;
    delete ballSensors;
}

unsigned int BallDetection::getBallAngle() {
    unsigned int tmp = 0;
    unsigned int maxConnected = 0;
    unsigned short maxAngleNum = error_value_angle;

    for (unsigned int i = 0; i < NUM_OF_SENSORS; i++) {
        if(ballSensors[i]->getStatus()) {
            tmp ++;
        }
        else {
            if (tmp > maxConnected) {
                maxConnected = tmp;
                maxAngleNum = i;
            }
            tmp = 0;
        }
    }

    maxAngleNum -= maxConnected/2;

    return IRSensor::Angle(maxAngleNum);
}