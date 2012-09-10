
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <string>

class StartingKnot;
class Data;

class Buffer {
    public:
        Buffer(unsigned short _maxElements = 5);
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

        virtual Data* getData(void) = 0;
        virtual Knot* getNext(void) = 0;

        virtual unsigned short getKnotCount(void) = 0;
};

class StartingKnot : public Knot {
    public:
        StartingKnot();
        ~StartingKnot();

        Knot* insert(Data* data);
        void releaseLastKnot();

        Data* getData(void);
        Knot* getNext(void);

    private:
        Knot* next;

    public:
        unsigned short getKnotCount(void);
};

class EndingKnot : public Knot {
    public:
        EndingKnot();
        ~EndingKnot();

        Knot* insert(Data* _data);
        Data* getData(void);

        Knot* getNext(void);

       unsigned short getKnotCount(void);
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

        static unsigned short knotCount;

    public:
        Knot* getNext(void);
        unsigned short getKnotCount(void);
};

#endif // _BUFFER_H_
