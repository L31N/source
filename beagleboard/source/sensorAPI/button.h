
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

#include "digitalSensor.h"

class Button : public DigitalSensor {
    public:
        Button(const std::string ipcName, const std::string canMember);
        ~Button();
};

#endif // _BUTTON_H_
