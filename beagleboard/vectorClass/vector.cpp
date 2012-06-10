
#include "vector.hpp"

/*
    Diese Methode erzeugt einen neuen Vector. Da kein Vektor angegeben wurde
    wir er als Nullvektor initialisier.
*/
vector::vector()
{
    x = 0;
    y = 0;
}


/*
    Diese Methode erzeugt einen neuen Vector mit den Koordinaten (px, py)
*/
vector::vector(double px, double py)
{
    x = px;
    y = py;
}


/*
    Diese Methode gibt den vector im Format ( x | y ) aus
*/
void vector::print()
{
    cout << "( " << x << " | " << y << ")";
}


/*
    Diese Methode gibt die X-Koordinate zurück
*/
double vector::getX()
{
    return x;
}


/*
    Diese Methode gibt die Y-Koordinate zurück
*/
double vector::getY()
{
    return y;
}


/*
    Diese Methode Addiert das Objekt und den vector sum
*/
vector vector::operator+(vector sum)
{
    x += sum.getX();
    y += sum.getY();

    return *this;
}


/*
    Diese Methode Addiert das Objekt und den vector sum
*/
vector vector::operator+=(vector sum)
{
    x += sum.getX();
    y += sum.getY();

    return *this;
}


/*
    Diese Methode Subtrahiert das Objekt und den vector sum
*/
vector vector::operator-(vector sum)
{
    x -= sum.getX();
    y -= sum.getY();

    return *this;
}


/*
    Diese Methode Subtrahiert das Objekt und den vector sum
*/
vector vector::operator-=(vector sum)
{
    x -= sum.getX();
    y -= sum.getY();

    return *this;
}


/*
    Diese Methode weist dem Objekt die Koordinaten von sum zu
*/
vector vector::operator=(vector sum)
{
    x = sum.getX();
    y = sum.getY();

    return *this;
}


/*
    Diese Methode Multipliziert das Objekt und den Faktor factor
*/
vector vector::operator*(double factor)
{
    x *= factor;
    y *= factor;

    return *this;
}


/*
    Diese Methode Multipliziert das Objekt und den Faktor factor
*/
vector vector::operator*=(double factor)
{
    x *= factor;
    y *= factor;

    return *this;
}


/*
    Diese Methode Dividiert das Objekt durch den Faktor factor
*/
vector vector::operator/(double factor)
{
    x /= factor;
    y /= factor;

    return *this;
}


/*
    Diese Methode Dividiert das Objekt durch den Faktor factor
*/
vector vector::operator/=(double factor)
{
    x /= factor;
    y /= factor;

    return *this;
}


/*
    Diese Prüft ob das Objekt gleich vector vect ist, wenn ja dann true
*/
bool vector::operator==(vector vect)
{
    if(x==vect.getX() && y==vect.getY())
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
    Diese Prüft ob das Objekt ungleich vector vect ist, wenn ja dann true
*/
bool vector::operator!=(vector vect)
{
    if(x!=vect.getX() || y!=vect.getY())
    {
        return true;
    }
    else
    {
        return false;
    }
}





