#ifndef _BBVECTOR_H_INCLUDED_
#define _BBVECTOR_H_INCLUDED_

#include <iostream>
#include <string>
#include <cmath>
#include <QString>

#include <cstring>

using namespace std;

class JVector
{
    public:
        JVector();
        JVector(double px, double  py);

    private:
        double x;
        double y;

    public:
        QString print();

        double getX();
        double getY();

        void setX(double px);
        void setY(double py);

        void set(double px, double py);

        // **********************************

        operator std::string();

		//***********************************


        JVector operator+(JVector sum);
        JVector operator+=(JVector sum);

        JVector operator-(JVector sum);
        JVector operator-=(JVector sum);

        JVector operator+(double l);
        JVector operator+=(double l);

        JVector operator-(double l);
        JVector operator-=(double l);

        JVector operator=(JVector sum);

        JVector operator*(double factor);
        JVector operator*=(double factor);

        JVector operator/(double factor);
        JVector operator/=(double factor);

        bool operator==(JVector vect);
        bool operator!=(JVector vect);

        double operator*(JVector vect);

        JVector operator++();
        JVector operator++(int);

        JVector operator--();
        JVector operator--(int);

        bool operator<(JVector vect);
        bool operator<=(JVector vect);

        bool operator>(JVector vect);
        bool operator>=(JVector vect);

        JVector operator!();

        //***********************************

        double abs();
        JVector getUnitVector();

        void setLength(double l);

        void setAngle(double angle, bool deg = true, bool fullCircle = true);
        void setAngle(JVector vect, double angle, bool deg = true, bool fullCircle = true);

        double getAngle(bool deg = true, bool fullCircle = true);
        double getAngle(JVector vect, bool deg = true, bool fullCircle = true);
};





#endif // _BBVECTOR_H_INCLUDED_


/* Operators to Implement

+ done
+= done

- done
-= done

= done

* double
*= double

/ double
/= double

== done
!=  done


* vector Skalarprodukt done
*= vector isn't possile

/ vector nothing for moment
/= vector nothing for moment


++ |vektor| ++ done
-- |vektor| -- done

< |vektor| < |vektor| done
<= |vektor| <= |vektor| done

> |vektor| > |vektor| done
>= |vektor| >= |vektor| done

! -vektor

% doen't make sense


getAngle();


*/
