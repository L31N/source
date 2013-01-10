
// motor.cpp

#include "motor.h"

Motor::Motor(const std::string _ipcName, const std::string _canMember) {
    ipcName = _ipcName;
    canMember = _canMember;

    speed = 0;

    can = new CAN(ipcName);
    can->init_member(canMember);
}

Motor::~Motor() {}

void Motor::setSpeed(short _speed) {
    speed = _speed;
    can->setValue(canMember, speed);
}

short Motor::getSpeed() { return speed; }
