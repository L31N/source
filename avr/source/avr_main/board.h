
#ifndef _BOARD_H_
#define _BOARD_H_

#include <avr/io.h>

class Board {
    public:
        Board();
        ~Board();

        static const unsigned char anybutton = 4;

        bool buttonDown(unsigned char num = anybutton);
        void ledOn(unsigned char num, bool on);

};

#endif // _BOARD_H_
