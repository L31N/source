#ifndef JFIELDVIEW_H
#define JFIELDVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPen>

#include "jvector.h"

class JFieldView : public QWidget
{
        Q_OBJECT
    public:
        JFieldView(QWidget *parent = 0);
        
    private:
        JVector size;

        JVector currpos;
        int currot;

        void paintEvent(QPaintEvent *);

    signals:
        
    public slots:
        void updatePos(int rot, JVector pos);
};

#endif // JFIELDVIEW_H
