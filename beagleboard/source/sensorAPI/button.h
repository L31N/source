
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

#include "digitalSensor.h"

class Button : CANSensor {
    public:
        Button(const std::string ipcName, const std::string canMember);
        ~Button();

        bool anybutton();
        bool getButton(unsigned char buttonnum);

    private:
        unsigned char status;
};

#endif // _BUTTON_H_
