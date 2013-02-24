
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "board.h"
#include "motor.h"
#include "motionController.h"
#include "can.h"

/*const unsigned char id_ir_front = 129;
const unsigned char id_ir_right = 135;
const unsigned char id_ir_back = 144;
const unsigned char id_ir_left = 150;

void init_avr();
bool button(unsigned char num = 5);     // 5 is anybutton
void set_ir_can_filter();
void motor(unsigned short num, signed short speed);
bool get_ir_value(unsigned char num);*/
//unsigned short get_ball_dir();


int main () {

    uart_init(38400);
    can_init(BITRATE_100_KBPS);
    sei();

    Board board;
    Motor motor;
    board.ledOn(7,true);

    MotionController mc;

    while(!board.buttonDown());
    board.ledOn(2,true);

    const short speed = 100;

    Vector vector (-1, -1);
    uart_debug("\n\rVector: %d | %d", vector.getX()*1000, vector.getY()*1000);
    vector.setLength(speed);
    uart_debug("\n\rVector: %d | %d", vector.getX()*1000, vector.getY()*1000);
    uart_debug("\n\rAngle: %d", vector.getAngle()*1000);
    mc.drive(vector);
    _delay_ms(1000);
    mc.pbreak();

    while(42);

    return 0;
}
