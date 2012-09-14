
#ifndef _ANGLE_H_
#define _ANGLE_H_

class Angle {
    public:
        Angle(int _value);
        ~Angle();

    private:
        int value;

    public:
        void setValue(int _value) {
            if (_value < 0) _value *= -1;
            value = _value%360;
        }

        int getValue(void) { return value; }
};

#endif //_ANGLE_H_
