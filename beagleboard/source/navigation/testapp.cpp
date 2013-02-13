
#include <cmath>
#include <iostream>

int main () {

    //static const unsigned int MAX_X = 1820;
    //static const unsigned int MAX_Y = 2430;
    const unsigned int MAX_X = 105;
    const unsigned int MAX_Y = 165;

    //static const unsigned int BOT_DIA = 220;
    //static const unsigned int BOT_RAD = 110;
    const unsigned int BOT_DIA = 0;
    const unsigned int BOT_RAD = 0;

    unsigned int x = 0;
    unsigned int y = 0;

    unsigned int laser_values[4];

    //for (int i = 0; i < 4; i++) {
        //laser_values[i] = laserSensors[i]->getDistance(LaserSensor::mm) + BOT_RAD;
    //}
    laser_values[0] = 89;
    laser_values[1] = 62;
    laser_values[2] = 96;
    laser_values[3] = 62;

    unsigned int d[2];
    unsigned int g[2];

    if (laser_values[0] + laser_values[2] <= laser_values[1] + laser_values[3]) {
        d[0] = laser_values[0];
        d[1] = laser_values[2];
        g[0] = laser_values[1];
        g[1] = laser_values[3];
    }
    else {
        d[0] = laser_values[1];
        d[1] = laser_values[3];
        g[0] = laser_values[0];
        g[1] = laser_values[2];
    }

    x = d[0]*cos(acos((MAX_X)/(d[0]+d[1])));
    y = g[0]*cos(acos((MAX_Y)/(g[0]+g[1])));

    std::cout << "d[0]: " << d[0] << std::endl;
    std::cout << "d[1]: " << d[1] << std::endl;

    std::cout << "g[0]: " << g[0] << std::endl;
    std::cout << "g[1]: " << g[1] << std::endl;

    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;

    return 0;
}
