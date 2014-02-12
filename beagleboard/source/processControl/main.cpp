
#include <iostream>

#include "sensor/gpio.h"
#include "debug/bbdebug.h"

#define NUM_BUTTON_0    69
#define NUM_BUTTON_1    66
#define NUM_BUTTON_2    67

#define NUM_LED_RED     45
#define NUM_LED_YELLOW  68
#define NUM_LED_GREEN   44


int main () {
    Debug debug("PROCESS_CONTROL_DBG");

    GPIO button0("USER_BUTTON0", NUM_BUTTON_0, GPIO::INPUT, GPIO::BOTH);
    GPIO button1("USER_BUTTON1", NUM_BUTTON_1, GPIO::INPUT, GPIO::BOTH);
    GPIO button2("USER_BUTTON2", NUM_BUTTON_2, GPIO::INPUT, GPIO::BOTH);

    GPIO led_red("LED0", NUM_LED_RED, GPIO::OUTPUT);
    GPIO led_yellow("LED1", NUM_LED_YELLOW, GPIO::OUTPUT);
    GPIO led_green("LED2", NUM_LED_GREEN, GPIO::OUTPUT);

    while (true) {
        debug.send("waiting for start button ...");
        button0.poll(-1);

        led_red.setValue(!led_red.getValue());
        led_green.setValue(!led_green.getValue());
        led_yellow.setValue(!led_yellow.getValue());

    }
}


/** functionality **/

// kill all processes, stop motors
// start all processes
// switch tactic
