#ifndef JDIRECTIONALPAD_H
#define JDIRECTIONALPAD_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QDebug>

class JDirectionalPad : public QWidget
{
        Q_OBJECT
    public:
        JDirectionalPad(QWidget *parent = 0);

        void setX(float px);
        void setY(float py);

    protected:
        void paintEvent(QPaintEvent *);

    private:
        float x;
        float y;

    signals:
        
    public slots:
        
};

#endif // JDIRECTIONALPAD_H
