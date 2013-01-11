
#ifndef _REFLEX_SWITCH_H_
#define _REFLEX_SWITCH_H_

#include <string>

#include "digitalSensor.h"

class ReflexSwitch : public DigitalSensor {
    public:
        ReflexSwitch(const std::string ipcName, const std::string canMember);
        ~ReflexSwitch();
};

#endif // _REFLEX_SWTICH_H_
