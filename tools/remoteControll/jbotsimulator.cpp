#include "jbotsimulator.h"

JBotSimulator::JBotSimulator(QObject *parent) : QObject(parent)
{
    rotspeed = 0;
    movvect.set(0,0);

    currrot = 0;
    size.set(200,100);
    currpos.set(size.getX()/2, size.getY()/2);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePos()));
    timer->start(1);
}

void JBotSimulator::setRotationSpeed(int speed)
{
    rotspeed = speed;
}

void JBotSimulator::setMovementDirection(JVector pvect)
{
    movvect = !pvect;
}

void JBotSimulator::setMovementX(float px)
{
    movvect.setX(-px);
}

void JBotSimulator::setMovementY(float py)
{
    movvect.setY(-py);
}

void JBotSimulator::setRotation(float rot)
{
    currrot = rot;
}


void JBotSimulator::updatePos()
{
    //Rotation berechnen
    currrot += 0.00141176*rotspeed;

    while(currrot > 360) currrot -= 360;
    while(currrot < 0) currrot += 360;

    //Movement berechnen
    JVector movement = movvect;
    movement /= 10;

    int newmoveangle = currrot + movement.getAngle();
    newmoveangle = newmoveangle % 360;
    newmoveangle = 360 - newmoveangle;

    movement.setAngle(newmoveangle);

    JVector newpos = currpos;
    newpos += movement;

    bool ok = true;

    if(newpos.getX() < 0) ok = false;
    else if(newpos.getX() > size.getX()) ok = false;
    else if(newpos.getY() < 0) ok = false;
    else if(newpos.getY() > size.getY()) ok = false;


    if(ok) currpos += movement;

    emit positionChanged(currrot, currpos);
}
