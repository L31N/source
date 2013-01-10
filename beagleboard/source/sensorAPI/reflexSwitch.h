
#ifndef _REFLEX_SWITCH_H_
#define _REFLEX_SWITCH_H_

#include <string>

#include "digitalSensor.h"

class ReflexSwitch : public DigitalSensor {
    public:
        ReflexSwitch(std::string ipcName, std::string canMember);
        ~ReflexSwitch();
};

#endif // _REFLEX_SWTICH_H_
