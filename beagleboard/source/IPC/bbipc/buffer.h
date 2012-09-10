
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <string>

class StartingKnot;
class Data;

class Buffer {
    public:
        Buffer(unsigned short _maxElements = 0);
        ~Buffer();

        void insert(Data* data);
        Data* getLastData(void);

    private:
        StartingKnot* start;

        unsigned short maxElements;
};

class Data {
    public:
        Data();
        ~Data();

    private:
        std::string data;
        short senderID;
};

class Knot {
    public:
        Knot() {}
        virtual ~Knot() {}

        virtual Knot* insert(Data* data) = 0;
};

class StartingKnot : public Knot {
    public:
        StartingKnot();
        ~StartingKnot();

        Knot* insert(Data* data);
        Data* getData(void);

    private:
        Knot* next;
};

class EndingKnot : public Knot {
    public:
        EndingKnot();
        ~EndingKnot();

        Knot* insert(Data* _data);
};

class DataKnot : public Knot {
    public:
        DataKnot(Data* _data, Knot* _knot);
        ~DataKnot();

        Knot* insert(Data* data);
        Data* getData(void);

    private:
        Data* data;
        Knot* next;
};

#endif // _BUFFER_H_
