
// button.cpp

#include "button.h"

Button::Button(std::string ipcName, std::string canMember) : DigitalSensor(ipcName, canMember) {}

Button::~Button() {}
