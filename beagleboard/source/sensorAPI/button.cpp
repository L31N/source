
// button.cpp

#include "button.h"

Button::Button(const std::string ipcName, const std::string canMember) : CANSensor(ipcName, canMember) {}

Button::~Button() {}

bool Button::anybutton() {
    if (can->checkForNewData()) {
        status = *(can->getValue());
    }

    return bool (status);
}

bool Button::getButton(unsigned char buttonnum) {

    if (buttonnum > 3) {
        std::cerr << "Button::getButton() --> invalid button id: " << buttonnum << std::endl;
        debug->send("Button::getButton() --> invalid button id: %d", buttonnum);
        return false;
    }

    if (can->checkForNewData()) {
        status = *(can->getValue());
    }

    return ((status >> 4+buttonnum) & 0x01);
}
