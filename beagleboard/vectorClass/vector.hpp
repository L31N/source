#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include <iostream>
#include <cmath>

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

        void setX(double px);
        void setY(double py);

		//***********************************


        vector operator+(vector sum);
        vector operator+=(vector sum);

        vector operator-(vector sum);
        vector operator-=(vector sum);

        vector operator+(double l);
        vector operator+=(double l);

        vector operator-(double l);
        vector operator-=(double l);

        vector operator=(vector sum);

        vector operator*(double factor);
        vector operator*=(double factor);

        vector operator/(double factor);
        vector operator/=(double factor);

        bool operator==(vector vect);
        bool operator!=(vector vect);

        double operator*(vector vect);

        vector operator++();
        vector operator++(int);

        vector operator--();
        vector operator--(int);

        bool operator<(vector vect);
        bool operator<=(vector vect);

        bool operator>(vector vect);
        bool operator>=(vector vect);

        vector operator!();

        //***********************************

        double abs();
        vector getUnitVector();

        void setLenght(double l);

        double getAngle(vector vect, bool deg = true);
        double getAngle(bool deg);


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
