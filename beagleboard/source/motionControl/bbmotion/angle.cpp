
// angle.cpp

#include "angle.h"

Angle::Angle(unsigned short _angle) { angle = _angle; }

Angle::Angle(short _angle) {
    if (_angle >= 0) angle = _angle % 360;
    else angle = 360 + _angle % 360;
}

Angle::Angle(unsigned int _angle) { angle = _angle % 360; }

Angle::Angle(int _angle) {
    if (_angle >= 0) angle = _angle % 360;
    else angle = 360 + _angle % 360;
}

Angle::Angle(unsigned long _angle) { angle = _angle % 360; }

Angle::Angle(long _angle) {
    if (_angle >= 0) angle = _angle % 360;
    else angle = 360 + _angle % 360;
}

Angle::Angle(float _angle) {
    if (_angle >= 0) angle = int(_angle) % 360;
    else angle = 360 + int(_angle) % 360;
}

Angle::Angle(double _angle) {
    if (_angle >= 0) angle = long(_angle) % 360;
    else angle = 360 + long(_angle) % 360;
}

Angle::~Angle() {}

unsigned short Angle::value() { return angle; }
