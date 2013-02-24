
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "board.h"
#include "uart.h"
#include "motor.h"
#include "motionController.h"
#include "can.h"

const short speed = 100;

int main () {

	//Schnittstellen initialisieren
    uart_init(38400);
    can_init(BITRATE_100_KBPS);
    sei();

	//Objekte erstellen
    Board board;
    MotionController mc;

	//Startenstellungen herstelln
    board.ledOn(7,true);
    mc.pbreak();
drive(Direction dir, short speed);
    board.ledOn(0,true);

	//Alle richtungen testen

	while(!board.buttonDown());
	mc.drive(FRONT, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(RIGHT, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(BACK, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(LEFT, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(FRONTRIGHT, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(BACKRIGHT, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(BACKLEFT, speed);
	_delay_ms(1000);
	mc.pbreak();

	while(!board.buttonDown());
	mc.drive(FRONTLEFT, speed);
	_delay_ms(1000);
	mc.pbreak();

}
