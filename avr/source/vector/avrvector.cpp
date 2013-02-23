
#include "avrvector.h"

/*
    Diese Methode erzeugt einen neuen Vector. Da kein Vektor angegeben wurde
    wir er als Nullvektor initialisier.
*/
Vector::Vector()
{
    x = 0;
    y = 0;
}


/*
    Diese Methode erzeugt einen neuen Vector mit den Koordinaten (px, py)
*/
Vector::Vector(double px, double py)
{
    x = px;
    y = py;
}

/*
    Dieser Konstruktor erlaubt das kopieren eines strings in einen Vector
*/
/*Vector::Vector(std::string str) {
    memcpy(this, str.c_str(), sizeof(*this));
}*/

/*
    Diese Methode gibt den vector im Format ( x | y ) aus
*/
/*void Vector::print()
{
    cout << "( " << x << " | " << y << ")";
}*/


/*
    Diese Methode gibt die X-Koordinate zurück
*/
double Vector::getX()
{
    return x;
}


/*
    Diese Methode gibt die Y-Koordinate zurück
*/
double Vector::getY()
{
    return y;
}


/*
    Setzt X
*/
void Vector::setX(double px)
{
    x = px;
    return;
}


/*
    Setzt Y
*/
void Vector::setY(double py)
{
    y = py;
    return;
}


/*
    Setzt x und y
*/
void Vector::set(double px, double py) {
    x = px;
    y = py;

    return;
}

/*
    Setzt die Länge auf l
*/
void Vector::setLength(double l)
{
    Vector tmp = this->getUnitVector()*l;
    (*this) = tmp;
}

//******************************************************************************

/*Vector::operator std::string() {
    return std::string((char*)this, sizeof(*this));
}*/

//******************************************************************************

/*
    Diese Methode Addiert das Objekt und den vector sum
*/

Vector Vector::operator+(Vector sum)
{
    x += sum.getX();
    y += sum.getY();

    return *this;
}


/*
    Diese Methode Addiert das Objekt und den vector sum
*/
Vector Vector::operator+=(Vector sum)
{
    x += sum.getX();
    y += sum.getY();

    return *this;
}


/*
    Diese Methode Subtrahiert das Objekt und den vector sum
*/
Vector Vector::operator-(Vector sum)
{
    x -= sum.getX();
    y -= sum.getY();

    return *this;
}


/*
    Diese Methode Subtrahiert das Objekt und den vector sum
*/
Vector Vector::operator-=(Vector sum)
{
    x -= sum.getX();
    y -= sum.getY();

    return *this;
}


/*
    Diese Methode verlängert das Objekt um l
*/
Vector Vector::operator+(double l)
{
    Vector tmp = this->getUnitVector()*l;
    (*this) += tmp;

    return *this;
}


/*
    Diese Methode verlängert das Objekt um l
*/
Vector Vector::operator+=(double l)
{
    Vector tmp = this->getUnitVector()*l;
    (*this) += tmp;

    return *this;
}


/*
    Diese Methode verkürzt das Objekt um l
*/
Vector Vector::operator-(double l)
{
    Vector tmp = this->getUnitVector()*l;
    (*this) -= tmp;

    return *this;
}


/*
    Diese Methode verkürzt das Objekt um l
*/
Vector Vector::operator-=(double l)
{
    Vector tmp = this->getUnitVector()*l;
    (*this) -= tmp;

    return *this;
}


/*
    Diese Methode weist dem Objekt die Koordinaten von sum zu
*/
Vector Vector::operator=(Vector sum)
{
    x = sum.getX();
    y = sum.getY();

    return *this;
}


/*
    Diese Methode Multipliziert das Objekt und den Faktor factor
*/
Vector Vector::operator*(double factor)
{
    x *= factor;
    y *= factor;

    return *this;
}


/*
    Diese Methode Multipliziert das Objekt und den Faktor factor
*/
Vector Vector::operator*=(double factor)
{
    x *= factor;
    y *= factor;

    return *this;
}


/*
    Diese Methode Dividiert das Objekt durch den Faktor factor
*/
Vector Vector::operator/(double factor)
{
    x /= factor;
    y /= factor;

    return *this;
}


/*
    Diese Methode Dividiert das Objekt durch den Faktor factor
*/
Vector Vector::operator/=(double factor)
{
    x /= factor;
    y /= factor;

    return *this;
}


/*
    Diese Prüft ob das Objekt gleich vector vect ist, wenn ja dann true
*/
bool Vector::operator==(Vector vect)
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
bool Vector::operator!=(Vector vect)
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
double Vector::operator*(Vector vect)
{
	return x*vect.getX()+y*vect.getY();
}


/*
    Erhöht den Betrag um 1
*/
Vector Vector::operator++()
{
	*this += this->getUnitVector();
	return *this;
}


/*
    Erhöht den Betrag um 1
*/
Vector Vector::operator++(int)
{
	Vector tmp = *this;
	*this += this->getUnitVector();
	return tmp;
}


/*
    Rediziert den Betrag um 1
*/
Vector Vector::operator--()
{
	*this -= this->getUnitVector();
	return *this;
}


/*
    Reduziert den Betrag um 1
*/
Vector Vector::operator--(int)
{
	Vector tmp = *this;
	*this -= this->getUnitVector();
	return tmp;
}


/*
    vergleicht die Beträge
*/
bool Vector::operator<(Vector vect)
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
bool Vector::operator<=(Vector vect)
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
bool Vector::operator>(Vector vect)
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
bool Vector::operator>=(Vector vect)
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
Vector Vector::operator!()
{
    Vector tmp = *this;
    tmp *= -1;
    return tmp;
}


//*******************************************************************

/*
    Gibt den Betrag des Vektors zurück
*/
double Vector::abs()
{
	return sqrt(x*x+y*y);
}


/*
    Gibt den Einheitsvektor zurück
*/
Vector Vector::getUnitVector()
{
	Vector tmp = *this;
	tmp /= tmp.abs();
	return tmp;
}


/*
    Gibt den Winkel zwischen this und dem Vektor v(0,1) zurück
*/
double Vector::getAngle(bool deg, bool fullCircle)
{
    double angleV1 = 0;

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

    if(angleV1 < 0 && fullCircle) angleV1 += 2*M_PI;

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
double Vector::getAngle(Vector vect, bool deg, bool fullCircle)
{
    double angleV1 = this->getAngle(false, false);
    double angleV2 = vect.getAngle(false, false);

    double angle = (angleV2 - angleV1);
    if(angle < 0 && fullCircle) angle += 2*M_PI;

    if(deg)
    {
        angle *= 180;
        angle /= M_PI;
    }

    return angle;
}

/*
	Setzt den Winkel zwischen this und dem Vektor v(0,1)
*/
void Vector::setAngle(double angle, bool deg, bool fullCircle)
{
	if(deg)
    {
    	angle *= M_PI;
        angle /= 180;
    }

    if(fullCircle && angle > M_PI) angle -= 2* M_PI;

	double absolute = this->abs();

	this->x = absolute*sin(angle);
	this->y = absolute*cos(angle);
}

/*
	Setzt den Winkel zwischen this und dem Vektor vect
*/
void Vector::setAngle(Vector vect, double angle, bool deg, bool fullCircle)
{
	if(deg)
    {
    	angle *= M_PI;
        angle /= 180;
    }

    if(fullCircle && angle > M_PI) angle -= 2* M_PI;

	double a = vect.getAngle(false);
	a += angle;
	if(a > M_PI) a = -2*M_PI+a;

	this->setAngle(a, false);
}



