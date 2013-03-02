
#ifndef _REFLEX_SWITCH_H_
#define _REFLEX_SWITCH_H_

#include <string>

#include "digitalSensor.h"

class ReflexSwitch : public DigitalSensor {
    public:
        ReflexSwitch(const std::string _ipcName, const std::string _canMember);
        ~ReflexSwitch();
};

#endif // _REFLEX_SWTICH_H_
