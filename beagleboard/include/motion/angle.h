
#ifndef _ANGLE_H_
#define _ANGLE_H_

class Angle {
    public:
        Angle(unsigned short _angle);
        Angle(short _angle);
        Angle(unsigned int _angle);
        Angle(int _angle);
        Angle(unsigned long _angle);
        Angle(long _angle);
        Angle(float _angle);
        Angle(double _angle);

        ~Angle();

        unsigned short value();

    private:
        unsigned short angle;
};

#endif // _ANGLE_H_
