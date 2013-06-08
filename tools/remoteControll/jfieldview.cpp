#include "jfieldview.h"

JFieldView::JFieldView(QWidget *parent) : QWidget(parent)
{
    size.set(200,100);

    currot = 0;
    currpos.set(size.getX()/2, size.getY()/2);

}

void JFieldView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0,0,this->width(), this->height(), Qt::green);

    QPen pen(Qt::gray);
    pen.setWidth(5);
    painter.setPen(pen);

    QPoint pos(currpos.getX()/size.getX()*this->width(),currpos.getY()/size.getY()*this->height());
    painter.drawEllipse(pos, 10, 10);

    JVector end(0,10);
    end.setAngle(currot);
    painter.drawLine(pos.x(), pos.y(), pos.x()+end.getX(), pos.y()-end.getY());
}

void JFieldView::updatePos(int rot, JVector pos)
{
    currot = rot;
    currpos = pos;
    this->repaint();
}
