#include "jdirectionalpad.h"

JDirectionalPad::JDirectionalPad(QWidget *parent) : QWidget(parent)
{
    x = 0.5;
    y = 0.5;

}

void JDirectionalPad::setX(float px)
{
    x = px+1;
    x /= 2;
    this->repaint();
}

void JDirectionalPad::setY(float py)
{
    y = py+1;
    y /= 2;
    this->repaint();
}

void JDirectionalPad::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //Hintergrund malen
    painter.fillRect(0,0,this->size().width(),this->size().height(), Qt::white);

    painter.drawLine(this->size().width()/2, 0.1*this->size().height(), this->size().width()/2, 0.9*this->size().height());
    painter.drawLine(int(0.1*this->size().width()), int(this->size().height()/2), int(0.9*this->size().width()), int(this->size().height()/2));
    painter.drawEllipse(QPoint(int(this->size().width()/2), int(this->size().height()/2)), int(0.25*this->size().width()), int(0.25*this->size().height()));

    QPen pen(Qt::red);
    pen.setWidth(4);
    painter.setPen(pen);

    QPoint center(x*this->size().width(), y*this->size().height());
    painter.drawEllipse(center, int(0.05*this->size().width()), int(0.05*this->size().height()));

    this->size();
}
