
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "board.h"
#include "uart.h"
#include "motor.h"
#include "motionController.h"
#include "can.h"
#include "irSensor.h"

const short speed = 50;

int main () {

	//Schnittstellen initialisieren
    uart_init(38400);
    can_init(BITRATE_100_KBPS);
    sei();

	//Objekte erstellen
    Board board;
    MotionController mc;
    Motor motor;

	//Startenstellungen herstelln
    board.ledOn(7,true);
    //mc.pbreak();
    board.ledOn(0,true);

    //IRSensor ir;

    //while(!board.buttonDown());

    _delay_ms(500);

    while(!board.buttonDown());
    _delay_ms(200);

    /*for (int j = 200; j > 0; j--) {
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 150);
        _delay_ms(500);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 0);
        _delay_ms(j);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, -150);
        _delay_ms(500);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 0);
        _delay_ms(j);
    }*/

    /*const int delay = 1000;

    while(true) {
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 150);
        _delay_ms(500);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 0);
        _delay_ms(delay);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, -150);
        _delay_ms(500);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 0);
        _delay_ms(delay);
    }*/

    //while(true) {



    //while(!board.buttonDown());

        //for(int i = 0; i < 4; i++) motor.setSpeed(i, 150);
        //_delay_ms(20);
        /*for (int i = 0; i < 4; i++) motor.setSpeed(i, 77);
        _delay_ms(20);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 50);
        _delay_ms(20);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 41);
        _delay_ms(20);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, 37);
        _delay_ms(20);*/
        //for(int i = 0; i < 4; i++) motor.setSpeed(i, 35);


        //_delay_ms(1000);
        //for (unsigned int i = 0; i < 4; i++) motor.setSpeed(i, 0);
        //_delay_ms(100);


        //for(int i = 0; i < 4; i++) motor.setSpeed(i, -150);
        //_delay_ms(20);
        /*for(int i = 0; i < 4; i++) motor.setSpeed(i, -77);
        _delay_ms(20);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, -50);
        _delay_ms(20);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, -41);
        _delay_ms(20);
        for(int i = 0; i < 4; i++) motor.setSpeed(i, -37);
        _delay_ms(20);*/
        //for(int i = 0; i < 4; i++) motor.setSpeed(i, -35);

        //_delay_ms(1000);
        //for (unsigned int i = 0; i < 4; i++) motor.setSpeed(i, 0);

    //}

    //while(true);

    //(255-speed)*pow(e, (-0.05)*x)+speed;


	//mc.drive(FRONT, speed);
	/*motors.setSpeed(0, 100);
	motors.setSpeed(1, 100);
	motors.setSpeed(2, 100);
	motors.setSpeed(3, 100);*/
	//while(!board.buttonDown());
	/*for(int i = 0; i < 1000; i++) {
        _delay_ms(1);
        board.ledSwitch(3);
	}*/

	/*_delay_ms(500);

	board.ledOn(6, true);
	motors.setSpeed(0, 0);
	motors.setSpeed(1, 0);
	motors.setSpeed(2, 0);
	motors.setSpeed(3, 0);*/

	//mc.pbreak();

	//while(true);

	/*motor.test(0);
	_delay_ms(500);
	motor.test(1);
	_delay_ms(500);
    motor.test(2);
	_delay_ms(500);
	motor.test(3);*/

	/*while(true) {
        mc.drive(FRONT, speed);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(1000);
        mc.drive(BACK, speed);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(1000);
	}*/

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

    /*while(true) {

        //uart_debug(" r:%d", ir.getStatus(1));
        //uart_debug(" b:%d", ir.getStatus(5));
        //uart_debug(" l:%d", ir.getStatus(7));
        //uart_debug(" f:%d", ir.getStatus(9));
        uart_debug(" 0:%d", ir.getStatus(0));

        //uart_debug("%d", ir.getStatus(0));

        uart_debug("\r\n");
        _delay_ms(5);
    }*/

    /*bool tmp = false;
    while(true) {
        bool status = ir.getStatus(7);
        uart_debug("tmp: %d\n\r", tmp);
        if (status) {      /// ball found
            mc.drive(FRONT, speed);
            _delay_ms(100);
            uart_debug("FRONT\n\r");
        }
        else {                      /// seek the ball
            mc.drive(ROTATERIGHT, speed-10);
            _delay_ms(5);
            mc.pbreak();
            uart_debug("ROTATERIGHT\n\r");
        }

        if (board.buttonDown()) {
            mc.pbreak();
            _delay_ms(1000);

            while(!board.buttonDown());

            _delay_ms(1000);
        }
        _delay_ms(5);
    }*/

   //while(true) {
        /*unsigned char values = 0;
        values |= (ir.getStatus(9) << 3);
        values |= (ir.getStatus(7) << 2);
        values |= (ir.getStatus(5) << 1);
        values |= (ir.getStatus(1) << 0);*/

        //board.ledSwitch(0);

        /*if (values == 0x0F) {
            //uart_debug("PBREAK");       // alles high
            mc.pbreak();
        }
        else if (values == 0x00) {      // ball aus
            //uart_debug("PBREAK");
            mc.pbreak();
        }
        else if (values == 0x01) {      // rechts
            //uart_debug("BACK");
            mc.drive(BACK, speed);
        }
        else if (values == 0x02) {      // hinten
            //uart_debug("LEFT");
            mc.drive(LEFT, speed);
        }
        else if (values == 0x03) {      // hinten rechts
            //uart_debug("LEFT");
            mc.drive(LEFT, speed);
        }
        else if (values == 0x04) {      // links
            //uart_debug("BACK");
            mc.drive(BACK, speed);
        }
        else if (values == 0x05) {      // hinten (nahe)
            //uart_debug("LEFT");
            mc.drive(LEFT, speed);
        }
        else if (values == 0x06) {      // hinten links
            //uart_debug("BACK");
            mc.drive(BACK, speed);
        }
        else if (values == 0x07) {      // hinten nahe
            //uart_debug("RIGTH");
            mc.drive(RIGHT, speed);
        }
        else if (values == 0x08) {      // links
            //uart_debug("BACK");
            mc.drive(BACK, speed);
        }
        else if (values == 0x09) {      // vorne rechts
            //uart_debug("FRONTRIGHT");
            mc.drive(FRONTRIGHT, speed);
        }
        else if (values == 0x0A) {      // rechts und links
            //uart_debug("PBREAK");
            mc.pbreak();
        }
        else if (values == 0x0B) {      // rechts
            //uart_debug("BACK");
            mc.drive(BACK, speed);
        }
        else if (values == 0x0C) {      // vorne links
            //uart_debug("LEFT");
            mc.drive(LEFT, speed);
        }
        else if (values == 0x0D) {      // vorne
            //uart_debug("FRONT");
            mc.drive(FRONT, speed);
        }
        else if (values == 0x0E) {      // hinten
            //uart_debug("RIGHT");
            mc.drive(RIGHT, speed);
        }
        else {
            //uart_debug("ERROR UNKNOWN CASE");
        }*/

        /*if (board.buttonDown()) {
            mc.pbreak();
            _delay_ms(500);
            while(!board.buttonDown());
            _delay_ms(500);
        }*/

        //uart_debug("\n\r");
        //_delay_ms(50);

    //}
}


//Alle richtungen testen

	/*while(!board.buttonDown());
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
	*/
