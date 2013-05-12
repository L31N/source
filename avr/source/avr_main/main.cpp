
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

const short speed = 60;

int main () {

	//Schnittstellen initialisieren
    uart_init(38400);
    uart_putstr((char*)"uart init ...\n\r");

    can_init(BITRATE_10_KBPS);
    sei();

	//Objekte erstellen
    Board board;
    board.ledOn(0, true);

    /*can_t message;
    message.id = 0x01;
    message.flags.rtr = 0;
    message.length = 3;

    message.data[0] = 'L';
    message.data[1] = '3';
    message.data[2] = '1';
    message.data[3] = 'N';
    message.data[4] = '4';
    message.data[5] = '2';
    message.data[6] = 'P';
    message.data[7] = 'I';

    while(true){
        while(!board.buttonDown(1));
        can_send_message(&message);
        board.ledSwitch(3);
        board.ledSwitch(4);
        _delay_ms(500);
    }

    while(true);*/








    /*

//    message.data[0] = '0';
//    message.data[1] = '1';
//    message.data[2] = '2';
//    message.data[3] = '3';
//    message.data[4] = '4';
//    message.data[5] = '5';
//    message.data[6] = '6';
//    message.data[7] = '7';


    while(true) {
        if (board.buttonDown()) {
            can_send_message(&message);
            board.ledSwitch(3);
            _delay_ms(1000);
        }
    }*/

//    can_filter_t filter0;
//    filter0.id = 1;
//    filter0.mask = 0x0;
//    filter0.flags.rtr = 0;
//
//    can_set_filter(0, &filter0);
//
//    can_t message;
//    message.id = 0x01;
//    message.flags.rtr = 0;
//    message.length = 1;
//    message.data[0] = 0xFF;
//
//    can_send_message(&message);
//
//    while(true) {
//        if (can_check_message()) {
//            can_t msg;
//            can_get_message(&msg);
//            for (int i = 0; i < msg.length; i++) uart_putc(msg.data[i]);
//            uart_putc(' ');
//            board.ledSwitch(1);
//            board.ledSwitch(2);
//            board.ledSwitch(3);
//            board.ledSwitch(4);
//        }
//    }











    MotionController mc;
    Motor motor;

	//Startenstellungen herstelln
    board.ledOn(7,true);
    //mc.pbreak();
    board.ledOn(0,true);

    //IRSensor ir;

    //while(!board.buttonDown());

    _delay_ms(500);

    while(true) {
        while(!board.buttonDown(1));
        motor.setSpeed(150, 150, 150, 150);
        _delay_ms(1000);
        motor.setSpeed(0, 0, 0, 0);
    }

    //while(!board.buttonDown());
    //_delay_ms(200);

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
        for(int i = 0; i < 4;   guint id = g_io_add_watch(micbutton, G_IO_IN, mic_button_callback, NULL);i++) motor.setSpeed(i, -50);
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

    /*while(!board.buttonDown());
    motor.setSpeed(100, 100, 100, 100);
    _delay_ms(1000);
    motor.setSpeed(-100, -100, -100, -100);
    _delay_ms(60);
    motor.setSpeed(0, 0, 0, 0);*/

    /*while(!board.buttonDown());
    motor.test(0);
    motor.test(1);
    motor.test(2);
    motor.test(3);*/

    /*while(true) {
        while(!board.buttonDown());
        mc.drive(FRONT, speed);
        _delay_ms(1000);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(LEFT, speed);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(BACK, speed);
        _delay_ms(1000);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(RIGHT, speed);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(500);
    }*/

    /*while(true) {
        while(!board.buttonDown());
        mc.drive(FRONT, 255);
        //motor.setSpeed(speed, speed, speed, speed);
        _delay_ms(1000);
        motor.setSpeed(0, 0, 0, 0);
        //mc.drive(FRONT, speed/2);

        _delay_ms(400);
        mc.drive(BACK, speed);
        _delay_ms(1000);
        mc.pbreak();
    }*/

    /*while(true) {
        if (board.buttonDown(3)) {
            motor.setSpeed(0, 0, 0, 0);
            board.ledOn(0, 1);
            board.ledOn(1, 0);
            board.ledOn(2, 0);
            _delay_ms(500);
        }

        if (board.buttonDown(1)) {
            motor.setSpeed(speed, 0, 0, 0);
            board.ledOn(0, 0);
            board.ledOn(1, 1);
            board.ledOn(2, 0);
            _delay_ms(500);
        }

        if (board.buttonDown(2)) {
            motor.setSpeed(-speed, 0, 0, 0);
            board.ledOn(0, 0);
            board.ledOn(1, 0);
            board.ledOn(2, 1);
            _delay_ms(500);
        }
        if (board.buttonDown(0)) {
            motor.setSpeed(speed/2, 0, 0, 0);
            board.ledOn(0, 0);
            board.ledOn(1, 1);
            board.ledOn(2, 0);
            _delay_ms(500);
        }
        _delay_ms(5);
    }*/




    /*while(!board.buttonDown());
    mc.drive(FRONT, speed);
    _delay_ms(1000);
    mc.pbreak();*/

    /*while(true) {
        mc.drive(FRONT, 120);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(RIGHT, 120);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(BACK, 120);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(LEFT, 120);
        _delay_ms(500);
        mc.pbreak();
        _delay_ms(500);
    }*/

    while(!board.buttonDown());
    for (int i = 0; i < 3; i++) {
        mc.drive(FRONTRIGHT, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(200);
        mc.drive(BACKRIGHT, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(200);
        mc.drive(BACKLEFT, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(200);
        mc.drive(FRONTLEFT, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(200);
    }
        mc.drive(BACKRIGHT, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(500);
        mc.drive(FRONT, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(300);
        mc.drive(BACK, speed);
        _delay_ms(800);
        mc.pbreak();
        _delay_ms(300);

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
