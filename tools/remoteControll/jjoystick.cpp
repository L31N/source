#include "jjoystick.h"

JJoystick::JJoystick(QObject *parent) : QThread(parent)
{
    SDL_Init(SDL_INIT_JOYSTICK);

    if(SDL_NumJoysticks() < 1) return;

    //Joystick öffnen
    stick = SDL_JoystickOpen(0);

    sticks = new long[SDL_JoystickNumAxes(stick)];
    balls = new QPoint[SDL_JoystickNumBalls(stick)];
    hats = new unsigned char[SDL_JoystickNumHats(stick)];
    buttons = new bool[SDL_JoystickNumButtons(stick)];

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(1);


    this->start();
}

JJoystick::~JJoystick()
{
    if(SDL_JoystickOpened(0)) SDL_JoystickClose(stick);
    SDL_Quit();
}

void JJoystick::timeout()
{

    SDL_JoystickUpdate();

    for(int i = 0 ; i < SDL_JoystickNumAxes(stick) ; i++)
    {
        if(SDL_JoystickGetAxis(stick, i) != sticks[i])
        {
            sticks[i] = SDL_JoystickGetAxis(stick, i);
            emit axisChanged(i, sticks[i]/JJoystick::MAX_AXIS);
        }
    }

    for(int i = 0 ; i < SDL_JoystickNumBalls(stick) ; i++)
    {
        int dx, dy;
        SDL_JoystickGetBall(stick, i, &dx, &dy);

        if(QPoint(dx, dy) != balls[i])
        {
            balls[i] = QPoint(dx, dy);
            emit ballChanged(i, dx, dy);
        }
    }

    for(int i = 0 ; i < SDL_JoystickNumHats(stick) ; i++)
    {
        if(SDL_JoystickGetHat(stick, i) != hats[i])
        {
            hats[i] = SDL_JoystickGetHat(stick, i);
            emit hatChanged(i, hats[i]);
        }
    }

    for(int i = 0 ; i < SDL_JoystickNumButtons(stick) ; i++)
    {
        if(SDL_JoystickGetButton(stick, i) != buttons[i])
        {
            buttons[i] = SDL_JoystickGetButton(stick, i);
            emit buttonChanged(i, buttons[i]);
        }
    }

    return;

}

void JJoystick::check()
{
    if(SDL_JoystickOpened(0)) emit statusChanged("Joystick opened sucessfully!");
    else emit statusChanged("An error occured during opening of the joystick");
}
