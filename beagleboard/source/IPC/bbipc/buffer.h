
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <string>

class StartingNode;
class Data;

class Buffer {
    public:
        Buffer(unsigned short _maxElements = 5);
        ~Buffer();

        void insert(Data* data);
        Data* getLastData(void);

    private:
        StartingNode* start;

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

class Node {
    public:
        Node() {}
        virtual ~Node() {}

        virtual Node* insert(Data* data) = 0;

        virtual Data* getData(void) = 0;
        virtual Node* getNext(void) = 0;

        virtual unsigned short getNodeCount(void) = 0;
};

class StartingNode : public Node {
    public:
        StartingNode();
        ~StartingNode();

        Node* insert(Data* data);
        void releaseLastNode();

        Data* getData(void);
        Node* getNext(void);

    private:
        Node* next;

    public:
        unsigned short getNodeCount(void);
};

class EndingNode : public Node {
    public:
        EndingNode();
        ~EndingNode();

        Node* insert(Data* _data);
        Data* getData(void);

        Node* getNext(void);

       unsigned short getNodeCount(void);
};

class DataNode : public Node {
    public:
        DataNode(Data* _data, Node* _Node);
        ~DataNode();

        Node* insert(Data* data);
        Data* getData(void);

    private:
        Data* data;
        Node* next;

        static unsigned short NodeCount;

    public:
        Node* getNext(void);
        unsigned short getNodeCount(void);
};

#endif // _BUFFER_H_
