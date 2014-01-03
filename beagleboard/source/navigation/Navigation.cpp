#include "Navigation.h"

Navigation::Navigation() {

    positions = new Position[4];

    #warning Names must be set
    laserSensor[0] = new LaserSensor("", "LASER_SENSOR0", 0);
    laserSensor[1] = new LaserSensor("", "LASER_SENSOR1", 1);
    laserSensor[2] = new LaserSensor("", "LASER_SENSOR2", 2);
    laserSensor[3] = new LaserSensor("", "LASER_SENSOR3", 3);

    gyroSensor = new GyroSensor("GYRO");

    debug = new Debug("NAVIGATION");

    rcon = new ipcReceivingConnection("NAVIGATION");

    for (int i = 0; i < 4; i++)
        vectors[i] = Vector(0, 0);

    for (int i = 0; i < 10; i++)
        lastPositions[i] = Vector(0, 0);
}

Navigation::~Navigation() {
    delete positions;

    for (int i = 0; i < 4; i++)
        delete laserSensor[i];

    delete debug;
    delete rcon;
}

/** main run-loop **/
void Navigation::run() {
    /** start ranking procedure **/
    while(rcon->checkForNewData()) {
        vectors[3] = vectors[2];
        vectors[2] = vectors[1];
        vectors[1] = vectors[0];
        vectors[0] = Vector(rcon->readDataFromBuffer()->getData());
    }

    for (int i = 0; i < 4; i++) {
        rankByPosition(&positions[i]);
        rankByVector(&positions[i]);
        rankByTrend(&positions[i]);
    }

    Position chosen = choosePosition(positions);

    // update lastPositions
    for (int i = 10; i > 0; i++)
        lastPositions[i] = lastPositions[i-1];

    lastPositions[0] = chosen.getVector();

    /** end ranking **/
}




/** private member functions **/

void Navigation::rankByPosition(Position* position) {
    Vector line = position->getVector() - lastPositions[0];
    double distance = line.abs();
    position->setPositionRanking(distance);
}

void Navigation::rankByVector(Position* position) {
    Vector line = position->getVector() - lastPositions[0];

    Vector driveVector(1, 0);
    driveVector.setAngle(vectors[0].getAngle() + gyroSensor->getVector().getAngle());

    unsigned short angle = line.getAngle(driveVector);

    angle *= (vectors[0].abs() / 20);    // so the priority of the vector is higher the higher motor-speed is.

    position->setVectorRanking(angle);
}

void Navigation::rankByTrend(Position* postion) {
    // not implemented yet
}

Position Navigation::choosePosition(Position _positions[4]) {

    // set ranking priorities here -> the bigger the value the more influence this criterion has.
    const unsigned char positionPriority = 60;
    const unsigned char vectorPriority = 40;
    //const unsigned char trendPriority = 1;

    unsigned long long int rankings[4] = {
        positionPriority * _positions[0].getPositionRanking() + vectorPriority * _positions[0].getVectorRanking(),
        positionPriority * _positions[1].getPositionRanking() + vectorPriority * _positions[1].getVectorRanking(),
        positionPriority * _positions[2].getPositionRanking() + vectorPriority * _positions[2].getVectorRanking(),
        positionPriority * _positions[3].getPositionRanking() + vectorPriority * _positions[3].getVectorRanking()
    };

    unsigned char bestPosition = 0;

    // find smallest RankingValue
    for (int i = 1; i < 3; i++) {
        if (rankings[i] < rankings[bestPosition]) bestPosition = i;
    }

    return _positions[bestPosition];
}
