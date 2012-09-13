
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
            if (_value >= 0 && value <= 360) value = _value;
            else value = -1;
        }

        int getValue(void) { return value; }
};

#endif //_ANGLE_H_
