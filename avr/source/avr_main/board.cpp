
// board.cpp

#include "board.h"

Board::Board() {
    DDRA = 0xFF;
    DDRE &= ~(0xF0);

    PORTE |= 0xF0;
}

Board::~Board() {}

bool Board::buttonDown(unsigned char num) {
    if (num == anybutton) {
        if (PINE < 0xF0) return true;
        else return false;
    }
    else if (num <= 3) {
        if (~PINE & (1 << (4 + num))) return true;
        else return false;
    }
    else {
        // wrong number
        return false;
    }
}

void Board::ledOn(unsigned char num, bool on) {
    if(num > 7) return;

    if (on) PORTA |= (1 << num);
    else PORTA &= ~(1 << num);
}


