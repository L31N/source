#ifndef JJOYSTICK_H
#define JJOYSTICK_H

#include <QThread>
#include <QDebug>
#include <QPoint>
#include <QTimer>

#include <SDL/SDL.h>

class JJoystick : public QThread
{
        Q_OBJECT
    public:
        JJoystick(QObject *parent = 0);
        ~JJoystick();

    private slots:
        void timeout();

    public:
        static const unsigned char HAT_CENTERED = SDL_HAT_CENTERED;
        static const unsigned char HAT_UP = SDL_HAT_UP;
        static const unsigned char HAT_RIGHT = SDL_HAT_RIGHT;
        static const unsigned char HAT_DOWN = SDL_HAT_DOWN;
        static const unsigned char HAT_LEFT = SDL_HAT_LEFT;
        static const unsigned char HAT_RIGHTUP = SDL_HAT_RIGHTUP;
        static const unsigned char HAT_RIGHTDOWN = SDL_HAT_RIGHTDOWN;
        static const unsigned char HAT_LEFTUP = SDL_HAT_LEFTUP;
        static const unsigned char HAT_LEFTDOWN = SDL_HAT_LEFTDOWN;

        static const float MAX_AXIS = 32768.0;

        void check();

    private:
        SDL_Joystick *stick;
        long *sticks;
        QPoint *balls;
        unsigned char *hats;
        bool *buttons;
        QTimer *timer;

    signals:
        void axisChanged(int num, float value);
        void ballChanged(int num, int dx, int dy);
        void hatChanged(int num, unsigned char val);
        void buttonChanged(int num, bool pressed);
        void statusChanged(QString status);
        
    public slots:
        
};

#endif // JJOYSTICK_H
