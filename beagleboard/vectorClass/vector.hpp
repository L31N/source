#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include <iostream>

using namespace std;

class vector
{
    public:
        vector();
        vector(double px, double  py);

    private:
        double x;
        double y;

    public:
        void print();

        double getX();
        double getY();

        vector operator+(vector sum);
        vector operator+=(vector sum);

        vector operator-(vector sum);
        vector operator-=(vector sum);

        vector operator=(vector sum);

        vector operator*(double factor);
        vector operator*=(double factor);

        vector operator/(double factor);
        vector operator/=(double factor);

        bool operator==(vector vect);
        bool operator!=(vector vect);

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

* vector Skalrprodukt
*= vector isn't possile

/ vector nothing for moment
/= vector nothing for moment

++ |vektor| ++
-- |vektor| --

< |vektor| < |vektor|
<= |vektor| <= |vektor|

> |vektor| > |vektor|
>= |vektor| >= |vektor|

! -vektor
% doen't make sense





*/
