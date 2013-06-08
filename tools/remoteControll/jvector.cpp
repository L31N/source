
#include "jvector.h"

/*
    Diese Methode erzeugt einen neuen Vector. Da kein Vektor angegeben wurde
    wir er als Nullvektor initialisier.
*/
JVector::JVector()
{
    x = 0;
    y = 0;
}


/*
    Diese Methode erzeugt einen neuen Vector mit den Koordinaten (px, py)
*/
JVector::JVector(double px, double py)
{
    x = px;
    y = py;
}


/*
    Diese Methode gibt den vector im Format ( x | y ) aus
*/
QString JVector::print()
{
    QString tmp = "( ";
    tmp += QString::number(x);
    tmp += " | ";
    tmp += QString::number(y);
    tmp += ")";

    return tmp;
}


/*
    Diese Methode gibt die X-Koordinate zurück
*/
double JVector::getX()
{
    return x;
}


/*
    Diese Methode gibt die Y-Koordinate zurück
*/
double JVector::getY()
{
    return y;
}


/*
    Setzt X
*/
void JVector::setX(double px)
{
    x = px;
    return;
}


/*
    Setzt Y
*/
void JVector::setY(double py)
{
    y = py;
    return;
}


/*
    Setzt x und y
*/
void JVector::set(double px, double py) {
    x = px;
    y = py;

    return;
}

/*
    Setzt die Länge auf l
*/
void JVector::setLength(double l)
{
    JVector tmp = this->getUnitVector()*l;
    (*this) = tmp;
}

//******************************************************************************

JVector::operator std::string() {
    return std::string((char*)this, sizeof(*this));
}

//******************************************************************************

/*
    Diese Methode Addiert das Objekt und den vector sum
*/
JVector JVector::operator+(JVector sum)
{
    x += sum.getX();
    y += sum.getY();

    return *this;
}


/*
    Diese Methode Addiert das Objekt und den vector sum
*/
JVector JVector::operator+=(JVector sum)
{
    x += sum.getX();
    y += sum.getY();

    return *this;
}


/*
    Diese Methode Subtrahiert das Objekt und den vector sum
*/
JVector JVector::operator-(JVector sum)
{
    x -= sum.getX();
    y -= sum.getY();

    return *this;
}


/*
    Diese Methode Subtrahiert das Objekt und den vector sum
*/
JVector JVector::operator-=(JVector sum)
{
    x -= sum.getX();
    y -= sum.getY();

    return *this;
}


/*
    Diese Methode verlängert das Objekt um l
*/
JVector JVector::operator+(double l)
{
    JVector tmp = this->getUnitVector()*l;
    (*this) += tmp;

    return *this;
}


/*
    Diese Methode verlängert das Objekt um l
*/
JVector JVector::operator+=(double l)
{
    JVector tmp = this->getUnitVector()*l;
    (*this) += tmp;

    return *this;
}


/*
    Diese Methode verkürzt das Objekt um l
*/
JVector JVector::operator-(double l)
{
    JVector tmp = this->getUnitVector()*l;
    (*this) -= tmp;

    return *this;
}


/*
    Diese Methode verkürzt das Objekt um l
*/
JVector JVector::operator-=(double l)
{
    JVector tmp = this->getUnitVector()*l;
    (*this) -= tmp;

    return *this;
}


/*
    Diese Methode weist dem Objekt die Koordinaten von sum zu
*/
JVector JVector::operator=(JVector sum)
{
    x = sum.getX();
    y = sum.getY();

    return *this;
}


/*
    Diese Methode Multipliziert das Objekt und den Faktor factor
*/
JVector JVector::operator*(double factor)
{
    x *= factor;
    y *= factor;

    return *this;
}


/*
    Diese Methode Multipliziert das Objekt und den Faktor factor
*/
JVector JVector::operator*=(double factor)
{
    x *= factor;
    y *= factor;

    return *this;
}


/*
    Diese Methode Dividiert das Objekt durch den Faktor factor
*/
JVector JVector::operator/(double factor)
{
    x /= factor;
    y /= factor;

    return *this;
}


/*
    Diese Methode Dividiert das Objekt durch den Faktor factor
*/
JVector JVector::operator/=(double factor)
{
    x /= factor;
    y /= factor;

    return *this;
}


/*
    Diese Prüft ob das Objekt gleich vector vect ist, wenn ja dann true
*/
bool JVector::operator==(JVector vect)
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
bool JVector::operator!=(JVector vect)
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
double JVector::operator*(JVector vect)
{
	return x*vect.getX()+y*vect.getY();
}


/*
    Erhöht den Betrag um 1
*/
JVector JVector::operator++()
{
	*this += this->getUnitVector();
	return *this;
}


/*
    Erhöht den Betrag um 1
*/
JVector JVector::operator++(int)
{
    JVector tmp = *this;
	*this += this->getUnitVector();
	return tmp;
}


/*
    Rediziert den Betrag um 1
*/
JVector JVector::operator--()
{
	*this -= this->getUnitVector();
	return *this;
}


/*
    Reduziert den Betrag um 1
*/
JVector JVector::operator--(int)
{
    JVector tmp = *this;
	*this -= this->getUnitVector();
	return tmp;
}


/*
    vergleicht die Beträge
*/
bool JVector::operator<(JVector vect)
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
bool JVector::operator<=(JVector vect)
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
bool JVector::operator>(JVector vect)
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
bool JVector::operator>=(JVector vect)
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
JVector JVector::operator!()
{
    JVector tmp = *this;
    tmp *= -1;
    return tmp;
}


//*******************************************************************

/*
    Gibt den Betrag des Vektors zurück
*/
double JVector::abs()
{
	return sqrt(x*x+y*y);
}


/*
    Gibt den Einheitsvektor zurück
*/
JVector JVector::getUnitVector()
{
    JVector tmp = *this;
	tmp /= tmp.abs();
	return tmp;
}


/*
    Gibt den Winkel zwischen this und dem Vektor v(0,1) zurück
*/
double JVector::getAngle(bool deg, bool fullCircle)
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
double JVector::getAngle(JVector vect, bool deg, bool fullCircle)
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
void JVector::setAngle(double angle, bool deg, bool fullCircle)
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
void JVector::setAngle(JVector vect, double angle, bool deg, bool fullCircle)
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



