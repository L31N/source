
// button.cpp

#include "button.h"

Button::Button(const std::string ipcName, const std::string canMember) : DigitalSensor(ipcName, canMember) {}

Button::~Button() {}
