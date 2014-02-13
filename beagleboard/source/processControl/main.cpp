
#include <iostream>
#include <cstdlib>

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

    button0.getValue();
    button1.getValue();
    button2.getValue();

    std::system("./i2cd &");

    while (true) {
        debug.send("waiting for start button ...");
        led_green.setValue(true);
        button0.poll(-1);

        led_green.setValue(false);

        if(button2.getValue()) {
            led_yellow.setValue(true);
            std::system("killall i2cd");
            std::system("./i2cd &");
            sleep(2);
            led_yellow.setValue(false);
        }
        else if(button1.getValue()) {
            debug.send("Starting tactic for field");
            std::system("./tacticField &");
        }
        else {
            debug.send("Starting tactic for goal");
            std::system("./tacticGoal &");
        }

        usleep(200000);

        button0.getValue();

        led_red.setValue(true);

        debug.send("Programm is running waiting for termination");
        button0.poll(-1);

        debug.send("Terminating processes");

        std::system("killall tacticField");
        std::system("killall tacticGoal");
        std::system("cansend can0 009#0000000000000000");

        led_red.setValue(false);

        button0.getValue();

        usleep(200000);
    }
}


/** functionality **/

// kill all processes, stop motors
// start all processes
// switch tactic
