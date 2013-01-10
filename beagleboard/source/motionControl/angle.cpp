
// angle.cpp

#include "angle.h"

Angle::Angle(unsigned short _angle) { angle = _angle; }
Angle::Angle(short _angle) { angle = _angle % 360; }
Angle::Angle(unsigned int _angle) { angle = _angle % 360; }
Angle::Angle(int _angle) { angle = _angle % 360; }
Angle::Angle(unsigned long _angle) { angle = _angle % 360; }
Angle::Angle(long _angle) { angle = _angle % 360; }

Angle::Angle(float _angle) { angle = int(_angle) % 360; }
Angle::Angle(double _angle) { angle = long(_angle) % 360; }

Angle::~Angle() {}

unsigned short Angle::value() { return angle; }
