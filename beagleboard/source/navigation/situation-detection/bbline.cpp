
#include "bbline.h"

Line::Line()
{
    base.set(0,0);
    dir.set(0,0);
}

Line::Line(Vector pBase, Vector pDir)
{
    base = pBase;
    dir = pDir;
}

void Line::print()
{
    std::cout << "x = ";
    base.print();
    std::cout << " + r * ";
    dir.print();
}

Vector Line::getBase()
{
    return base;
}

Vector Line::getDir()
{
    return dir;
}

void Line::setBase(Vector pBase)
{
    base = pBase;
}

void Line::setDir(Vector pDir)
{
    dir = pDir;
}

void Line::set(Vector pBase, Vector pDir)
{
    base = pBase;
    dir = pDir;
}

bool Line::isValid()
{
    if(base.abs() != 0 && dir.abs() != 0) return true;
    else return false;
}

bool Line::isParallel(Line line)
{
    if(this->getDir().getX() / line.getDir().getX() != this->getDir().getY() / line.getDir().getY()) return false;
    return true;
}

bool Line::operator==(Line line)
{
    if(this->getDir().getX() / line.getDir().getX() != this->getDir().getY() / line.getDir().getY()) return false;
    if( ( ( line.getBase().getX() - this->getBase().getX() ) / this->getDir().getX() ) != ( ( line.getBase().getY() - this->getBase().getY() ) / this->getDir().getY() )) return false;
    return true;
}

bool Line::operator!=(Line line)
{
    return !(this->operator==(line));
}

/*********************************************************
This Function cuts this with the Line in Parameter line
The return-value ist the value for the Parameter r in
this, for which this is cut with line
*********************************************************/
double Line::cut(Line line)
{
    double a[2], b[2], c[2], d[2];

    a[0] = line.getBase().getX();
    a[1] = line.getBase().getY();

    b[0] = line.getDir().getX();
    b[1] = line.getDir().getY();

    c[0] = this->getBase().getX();
    c[1] = this->getBase().getY();

    d[0] = this->getDir().getX();
    d[1] = this->getDir().getY();

    return ( (b[1]*c[0] - a[0]*b[1] - b[0]*c[1] + a[1]*b[0]) / (-1.0*b[1]*d[0] + b[0]*d[1]));
}



