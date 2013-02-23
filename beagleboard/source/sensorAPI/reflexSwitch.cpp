
// reflexSwitch.cpp

#include "reflexSwitch.h"

ReflexSwitch::ReflexSwitch(const std::string _ipcName, const std::string _canMember) : DigitalSensor (_ipcName, _canMember) {}

ReflexSwitch::~ReflexSwitch() {}
