#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include <iostream>
#include <cmath>

using namespace std;

class Vector
{
    public:
        Vector();
        Vector(double px, double  py);

    private:
        double x;
        double y;

    public:
        void print();

        double getX();
        double getY();

        void setX(double px);
        void setY(double py);

		//***********************************


        Vector operator+(Vector sum);
        Vector operator+=(Vector sum);

        Vector operator-(Vector sum);
        Vector operator-=(Vector sum);

        Vector operator+(double l);
        Vector operator+=(double l);

        Vector operator-(double l);
        Vector operator-=(double l);

        Vector operator=(Vector sum);

        Vector operator*(double factor);
        Vector operator*=(double factor);

        Vector operator/(double factor);
        Vector operator/=(double factor);

        bool operator==(Vector vect);
        bool operator!=(Vector vect);

        double operator*(Vector vect);

        Vector operator++();
        Vector operator++(int);

        Vector operator--();
        Vector operator--(int);

        bool operator<(Vector vect);
        bool operator<=(Vector vect);

        bool operator>(Vector vect);
        bool operator>=(Vector vect);

        Vector operator!();

        //***********************************

        double abs();
        Vector getUnitVector();

        void setLenght(double l);

        double getAngle(bool deg = true);
        double getAngle(Vector vect, bool deg = true);
};





#endif // VECTOR_HPP_INCLUDED


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





*/
