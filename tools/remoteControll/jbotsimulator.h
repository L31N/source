#ifndef JBOTSIMULATOR_H
#define JBOTSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QString>

#include "jvector.h"

class JBotSimulator : public QObject
{
        Q_OBJECT
    public:
        JBotSimulator(QObject *parent = 0);
        
        void setRotationSpeed(int speed);
        void setMovementDirection(JVector pvect);
        void setMovementX(float px);
        void setMovementY(float py);

        void setRotation(float rot);
        void setSpeedMode(bool enabled);

    private:
        QTimer *timer;

        JVector movvect;
        int rotspeed;

        JVector currpos;
        float currrot;

        JVector size;

        bool speedMode;

    private slots:
        void updatePos();

    signals:
        void positionChanged(int rotation, JVector pos);


    public slots:
        
};

#endif // JBOTSIMULATOR_H
