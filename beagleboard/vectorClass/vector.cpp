
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
    Setzt X
*/
void vector::setX(double px)
{
    x = px;
    return;
}


/*
    Setzt Y
*/
void vector::setY(double py)
{
    y = py;
    return;
}


/*
    Setzt die Länge auf l
*/
void vector::setLenght(double l)
{
    vector tmp = this->getUnitVector()*l;
    (*this) = tmp;
}




//******************************************************************************

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
    Diese Methode verlängert das Objekt um l
*/
vector vector::operator+(double l)
{
    vector tmp = this->getUnitVector()*l;
    (*this) += tmp;

    return *this;
}


/*
    Diese Methode verlängert das Objekt um l
*/
vector vector::operator+=(double l)
{
    vector tmp = this->getUnitVector()*l;
    (*this) += tmp;

    return *this;
}


/*
    Diese Methode verkürzt das Objekt um l
*/
vector vector::operator-(double l)
{
    vector tmp = this->getUnitVector()*l;
    (*this) -= tmp;

    return *this;
}


/*
    Diese Methode verkürzt das Objekt um l
*/
vector vector::operator-=(double l)
{
    vector tmp = this->getUnitVector()*l;
    (*this) -= tmp;

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


/*
    Gibt das Skalarprodukt des Objekts und von vect zurück
*/
double vector::operator*(vector vect)
{
	return x*vect.getX()+y*vect.getY();
}


/*
    Erhöht den Betrag um 1
*/
vector vector::operator++()
{
	*this += this->getUnitVector();
	return *this;
}


/*
    Erhöht den Betrag um 1
*/
vector vector::operator++(int)
{
	vector tmp = *this;
	*this += this->getUnitVector();
	return tmp;
}


/*
    Rediziert den Betrag um 1
*/
vector vector::operator--()
{
	*this -= this->getUnitVector();
	return *this;
}


/*
    Reduziert den Betrag um 1
*/
vector vector::operator--(int)
{
	vector tmp = *this;
	*this -= this->getUnitVector();
	return tmp;
}


/*
    vergleicht die Beträge
*/
bool vector::operator<(vector vect)
{
    if(this->abs()<vect.abs())
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
    vergleicht die Beträge
*/
bool vector::operator<=(vector vect)
{
    if(this->abs()<=vect.abs())
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
    vergleicht die Beträge
*/
bool vector::operator>(vector vect)
{
    if(this->abs()>vect.abs())
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
    vergleicht die Beträge
*/
bool vector::operator>=(vector vect)
{
    if(this->abs()>=vect.abs())
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*
    gibt den Gegenvektor zurück
*/
vector vector::operator!()
{
    vector tmp = *this;
    tmp *= -1;
    return tmp;
}


//*******************************************************************

/*
    Gibt den Betrag des Vektors zurück
*/
double vector::abs()
{
	return sqrt(x*x+y*y);
}


/*
    Gibt den Einheitsvektor zurück
*/
vector vector::getUnitVector()
{
	vector tmp = *this;
	tmp /= tmp.abs();
	return tmp;
}


/*
    Gibt den Winkel zwischen this und dem Vektor v(0,1) zurück
*/
double vector::getAngle(bool deg)
{
    double angleV1;

    if(this->x > 0)
    {
        angleV1 = atan(y/x);
    }
    else if(this->x < 0 && this->y >= 0)
    {
        angleV1 = atan(this->y/this->x) + M_PI;
    }
    else if(this->x < 0 && this->y < 0)
    {
        angleV1 = atan(this->y/this->x) - M_PI;
    }
    else if(this->x == 0 && this->y > 0)
    {
        angleV1 = M_PI/2;
    }
    else if(this->x == 0 && this->y < 0)
    {
        angleV1 = -M_PI/2;
    }

    angleV1 *= -1;
    angleV1 += M_PI/2;

    if(angleV1 > M_PI)
    {
        angleV1 -= 2*M_PI;
    }

    if(deg)
    {
        angleV1 *= 180;
        angleV1 /= M_PI;
    }

    return angleV1;
}


/*
    Gibt den Winkel zwischen this und dem Vektor vect zurück
*/
double vector::getAngle(vector vect, bool deg)
{
    double angleV1 = this->getAngle(deg);
    double angleV2 = vect.getAngle(deg);

    return (angleV2 - angleV1);
}



