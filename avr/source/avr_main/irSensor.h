
#ifndef _IR_SENSOR_H_
#define _IR_SENSOR_H_

class IRSensor {
    public:
        static const unsigned char NUM_OF_SENSORS = 4;

        IRSensor();
        ~IRSensor();

        bool getStatus(unsigned char num);
        unsigned char getBallNum();

    private:
        unsigned long irValues;
};

#endif // _IR_SENSOR_H_
