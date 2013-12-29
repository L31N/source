#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <cmath>
#include <iostream>

#include "vector.h"

class Line
{
    public:
        Line();
        Line(Vector pBase, Vector pDir);

    private:
        Vector base;
        Vector dir;

    public:
        void print();

        Vector getBase();
        Vector getDir();

        void setBase(Vector pBase);
        void setDir(Vector pDir);

        void set(Vector pBase, Vector pDir);

        bool isValid();

        bool isParallel(Line line);

        bool operator==(Line line);
        bool operator!=(Line line);

        double cut(Line line);

};





#endif // LINE_H_INCLUDED
