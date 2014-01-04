#include <iostream>

#include "bbvector.h"
#include "bbline.h"

unsigned char detectSituation(Vector pPos, Vector pDir);

int main()
{
    Vector pos(750, 1250);
    Vector dir(1,1);

    unsigned char returnval = detectSituation(pos, dir);

    std::cout << "General: " << (returnval >> 4) << " Specific: " << (returnval & 0x0F) << std::endl;

    return 0;
}


unsigned char detectSituation(Vector pPos, Vector pDir)
{
    //Specifies the Size of the court
    unsigned long fieldWidth = 1500;
    unsigned long fieldHeight = 2500;

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
