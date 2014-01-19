
#include <unistd.h>

#include "navigation.h"

Navigation::Navigation() {

    reliabilityX = 0;
    reliabilityY = 0;

    debug = new Debug("NAVIGATION");

    laserSensor[0] = new LaserSensor("LASER_SENSOR0", "WENGLORS0", 0);
    laserSensor[1] = new LaserSensor("LASER_SENSOR1", "WENGLORS0", 1);
    laserSensor[2] = new LaserSensor("LASER_SENSOR2", "WENGLORS1", 0);
    laserSensor[3] = new LaserSensor("LASER_SENSOR3", "WENGLORS1", 1);

    gyro = new GyroSensor("GYRO");

    usleep(50000);
}

Navigation::~Navigation() {
    delete debug;
    for(int i = 0; i < 4; i++) delete laserSensor[i];
    delete gyro;
}

unsigned char Navigation::getReliabilityX() { return reliabilityX; }
unsigned char Navigation::getReliabilityY() { return reliabilityY; }
Vector Navigation::getPosition() { return position; }

void Navigation::calculate() {

    unsigned short distances[4];
    for (int i = 0; i < 4; i++) distances[i] = laserSensor[i]->getDistance();
    for (int i = 0; i < 4; i++) std::cout << distances[i] << std::endl;

    unsigned short distancesX[2];
    unsigned short distancesY[2];

    double alpha = gyro->getVector().getAngle(false, true);
    std::cout << "alpha: " << alpha << std::endl;

    Vector positions[4];

    unsigned int ialpha = (alpha/M_PI)*180;
    unsigned int specific_situation = (ialpha + 45) / 90 % 4;
    std::cout << "sps: " << specific_situation << std::endl;

    // calc all distances
    distancesX[0] = distances[(3 + specific_situation) % 4] * cos(alpha); // x1
    distancesX[1] = distances[(1 + specific_situation) % 4] * cos(alpha); // x2

    distancesY[0] = distances[(2 + specific_situation) % 4] * cos(alpha); // y1
    distancesY[1] = distances[(0 + specific_situation) % 4] * cos(alpha); // y2

    std::cout << "X0: " << distancesX[0] << std::endl;
    std::cout << "X1: " << distancesX[1] << std::endl;
    std::cout << "Y0: " << distancesY[0] << std::endl;
    std::cout << "Y1: " << distancesY[1] << std::endl;

    // calc possible positions
    positions[0] = Vector(FIELD_WIDTH - distancesX[1], FIELD_HEIGHT - distancesY[1]);
    positions[1] = Vector(FIELD_WIDTH - distancesX[1], distancesY[0]);
    positions[2] = Vector(distancesX[0], distancesY[0]);
    positions[3] = Vector(distancesX[0], FIELD_HEIGHT - distancesY[1]);

    for (int i = 0; i < 4; i++) {
        std::cout << "[" << i << "] ";
        positions[i].print();
        std::cout << " | ";
    }
    std::cout << std::endl;

    // calc average position
    Vector avPosition;
    avPosition.setX((positions[0].getX() + positions[1].getX() + positions[2].getX() + positions[3].getX()) / 4);
    avPosition.setY((positions[0].getY() + positions[1].getY() + positions[2].getY() + positions[3].getY()) / 4);

    std::cout << "avPosition: ";
    avPosition.print();
    std::cout << std::endl;

    // middle ranking
    Vector middle(FIELD_WIDTH/2, FIELD_HEIGHT/2);
    double middle_ranking = 1 - (middle-avPosition).abs() / middle.abs();
    middle_ranking = 343 + 7769 * middle_ranking;

    // set reliability values
    // distance - sum - reliability
    double variationX, variationY;
    variationX = abs((int(FIELD_WIDTH) - distancesX[0] - distancesX[1])) / double(FIELD_WIDTH);
    variationY = abs((int(FIELD_HEIGHT) - distancesY[0] - distancesY[1])) / double(FIELD_HEIGHT);

    std::cout << "variationX0: " << variationX << std::endl;
    std::cout << "variationX1: " << variationY << std::endl;

    variationX = 1.0 / (1.0+255.98*pow(M_E, -34.64*variationX));
    variationY = 1.0 / (1.0+255.98*pow(M_E, -34.64*variationY));

    std::cout << "variationX: " << variationX << std::endl;
    std::cout << "variationY: " << variationY << std::endl;

    // angle reliability
    std::cout << "ialpha: " << ialpha << std::endl;
    double angle_variation = ::abs(::abs((specific_situation * 90) - (ialpha+45)) % 90 - 45) / 45.0;
    std::cout << "angle_var0: " << angle_variation << std::endl;
    angle_variation = 1.0 / (1.0+middle_ranking*pow(M_E, -17.40*angle_variation));
    std::cout << "angle_var: " << angle_variation << std::endl;

    reliabilityX = 100*(1-angle_variation)*(1-variationX);
    reliabilityY = 100*(1-angle_variation)*(1-variationY);

    std::cout << "relX: " << int(reliabilityX) << std::endl;
    std::cout << "relY: " << int(reliabilityY) << std::endl;


    return;
}
