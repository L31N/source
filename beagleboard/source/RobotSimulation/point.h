
#ifndef _POINT_H_
#define _POINT_H_

class Point {
    public:
        Point(int _x, int _y);
        ~Point();

    private:
        int x;
        int y;

    public:
        int getX(void);
        int getY(void);

        void setX(int _x);
        void setY(int _y);
};

#endif //_POINT_H_
