#include "Navigation.h"

Navigation::Navigation() {

    laserSensor[0] = new LaserSensor("LASER_SENSOR0", "WENGLORS0", 0);
    laserSensor[1] = new LaserSensor("LASER_SENSOR1", "WENGLORS0", 1);
    laserSensor[2] = new LaserSensor("LASER_SENSOR2", "WENGLORS1", 0);
    laserSensor[3] = new LaserSensor("LASER_SENSOR3", "WENGLORS1", 1);

    gyroSensor = new GyroSensor("GYRO");
    gyroSensor->calibrate();
    sleep(2);

    debug = new Debug("NAVIGATION");

    rcon = new ipcReceivingConnection("NAVIGATION");

    for (int i = 0; i < 4; i++)
        vectors[i] = Vector(100, 100);

    for (int i = 0; i < 10; i++)
        lastPositions[i] = Vector(100, 100);

}

Navigation::~Navigation() {
    for (int i = 0; i < 4; i++)
        delete laserSensor[i];
    delete debug;
    delete rcon;
}

/** main run-loop **/
void Navigation::run() {

    while(true) {

        //lastPositions[0].print();
        //gyroSensor->getVector().print();
        //std::cout << std::flush;

        unsigned char situation = detectSituation(lastPositions[0], gyroSensor->getVector());
        calcPositions(situation);

        /*for (int i = 0; i < 4; i++) {
            positions[i].getVector().print();
            std::cout << "\t|\t";
        }
        std::cout << std::endl;*/


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
        for (int i = 10; i > 0; i--)
            lastPositions[i] = lastPositions[i-1];

        lastPositions[0] = chosen.getVector();

        /** end ranking **/
    }
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

unsigned char Navigation::detectSituation(Vector pPos, Vector pDir)
{
    //Specifies the Size of the court
    const unsigned long fieldWidth = FIELD_WIDTH;
    const unsigned long fieldHeight = FIELD_HEIGHT;

    //Create lines for the court
    Line leftBorder(Vector(0,0), Vector(0,1));
    Line lowerBorder(Vector(0,0), Vector(1,0));
    Line rightBorder(Vector(fieldWidth,0), Vector(0,1));
    Line upperBorder(Vector(0, fieldHeight), Vector(1,0));

    //Create lines for the laser sensors
    Line sensors[4];
    sensors[0].set(pPos, pDir);
    pDir.setAngle(pDir, 90);
    sensors[1].set(pPos, pDir);
    pDir.setAngle(pDir, 90);
    sensors[2].set(pPos, pDir);
    pDir.setAngle(pDir, 90);
    sensors[3].set(pPos, pDir);

    //Variables which counts the intersections, for each Border
    unsigned int count[4] = {0};

    //Array which stores if a sensor hits a wall in form strikes[sensor][wall]
    bool strikes[4][4];
    for(int i = 0; i < 4 ; i++) for(int j = 0; j < 4; j++) strikes[i][j] = false;

    //Check Borders
    for(int i=0; i<4; i++)
    {
        if(!leftBorder.isParallel(sensors[i]))
        {
            if(leftBorder.cut(sensors[i]) <= fieldHeight && leftBorder.cut(sensors[i]) >= 0 && sensors[i].cut(leftBorder) >= 0)
            {
                count[0]++;
                strikes[i][0] = true;
            }
        }
    }
    for(int i=0; i<4; i++)
    {
        if(!lowerBorder.isParallel(sensors[i]))
        {
            if(lowerBorder.cut(sensors[i]) <= fieldWidth && lowerBorder.cut(sensors[i]) >= 0 && sensors[i].cut(lowerBorder) >= 0)
            {
                count[1]++;
                strikes[i][1] = true;
            }
        }
    }
    for(int i=0; i<4; i++)
    {
        if(!rightBorder.isParallel(sensors[i]))
        {
            if(rightBorder.cut(sensors[i]) <= fieldHeight && rightBorder.cut(sensors[i]) >= 0 && sensors[i].cut(rightBorder) >= 0)
            {
                count[2]++;
                strikes[i][2] = true;
            }
        }
    }
    for(int i=0; i<4; i++)
    {
        if(!upperBorder.isParallel(sensors[i]))
        {
            if(upperBorder.cut(sensors[i]) <= fieldWidth && upperBorder.cut(sensors[i]) >= 0 && sensors[i].cut(upperBorder) >= 0)
            {
                count[3]++;
                strikes[i][3] = true;
            }
        }
    }

    //Count how much Borders are cut 0,1,2 Times
    unsigned int counter[3] = {0};

    for(int i = 0; i < 4; i++)
    {
        if(count[i] <= 2)
        {
            counter[count[i]]++;
        }
    }

    //Variables for Situation
    unsigned int generalSituation = 5;
    unsigned int specificSituation = 5;

    //Check general situation
    if(counter[1] == 4)
    {
        generalSituation = 0;

        if(strikes[0][3]) specificSituation = 0;
        else if(strikes[0][2]) specificSituation = 1;
        else if(strikes[0][1]) specificSituation = 2;
        else if(strikes[0][0]) specificSituation = 3;
    }
    else if(counter[1] == 2 && counter[2] == 1)
    {
        generalSituation = 1;

        if(count[3] == 2) specificSituation = 0;
        else if(count[2] == 2) specificSituation = 1;
        else if(count[1] == 2) specificSituation = 2;
        else if(count[0] == 2) specificSituation = 3;
    }
    else if(counter[2] == 2)
    {
        generalSituation = 2;

        if(strikes[0][2] && strikes[1][2]) specificSituation = 0;
        else if(strikes[0][2] && strikes[3][2]) specificSituation = 1;
        else if(strikes[2][2] && strikes[3][2]) specificSituation = 2;
        else if(strikes[2][2] && strikes[1][2]) specificSituation = 3;
    }

    unsigned short returnval = 0;
    returnval |= specificSituation;
    returnval |= (generalSituation << 4);

    return returnval;
}

void Navigation::calcPositions(unsigned char situation) {
    unsigned char generalSituation = situation >> 4;
    unsigned char specificSituation = situation & 0x0F;

    /// one wall per sensor

    unsigned short distances[4];
    for (int i = 0; i < 4; i++) distances[i] = laserSensor[i]->getDistance();

    unsigned short distancesX[2];
    unsigned short distancesY[2];

    //double alpha = gyroSensor->getVector().getAngle(false, true);
    double alpha = 0;

    //std::cout << "general: " << int(generalSituation) << "\tspecific: " << int(specificSituation) << std::endl;

    if (generalSituation == 0) {
        // calc all distances
        distancesX[0] = distances[(3 + specificSituation) % 4] * cos(alpha); // x1
        distancesX[1] = distances[(1 + specificSituation) % 4] * cos(alpha); // x2

        distancesY[0] = distances[(2 + specificSituation) % 4] * cos(alpha); // y1
        distancesY[1] = distances[(0 + specificSituation) % 4] * cos(alpha); // y2

        std::cout << "X1: " << distancesX[0] << "\tX2: " << distancesX[1];
        std::cout << "\tY1: " << distancesY[0] << "\tY2: " << distancesY[1] << std::endl;
        std::cout << "Lasers:";
        for (int i = 0; i < 4; i++) std::cout << "[" << i << "]: " << distances[i] << "\t\t";
        std::cout << std::endl;
        std::cout << "---------------------" << std::endl;

        // calc possible positions
        positions[0].setVector(Vector(FIELD_WIDTH - distancesX[1], FIELD_HEIGHT - distancesY[0]));
        positions[1].setVector(Vector(FIELD_WIDTH - distancesX[1], distancesY[0]));
        positions[2].setVector(Vector(distancesX[0], distancesY[0]));
        positions[3].setVector(Vector(distancesX[0], FIELD_HEIGHT - distancesY[0]));
    }

    return;
}
