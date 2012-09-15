
#include "buffer.h"

/** CLASS BUFFER **/
Buffer::Buffer(unsigned short _maxElements) { maxElements = _maxElements; }
Buffer::~Buffer() {}

void Buffer::insert(Data* data) {
    start->insert(data);

    /// To many elements exists ... delete the latest
    if (start->getNodeCount() > maxElements) start->releaseLastNode();

    return;
}

Data* Buffer::getLastData(void) { return start->getData(); }

/** CLASS DATA **/
Data::Data() {}
Data::~Data() {}

std::string Data::getData(void) { return data; }
short Data::getSenderID(void) { return senderID; }

/** CLASS STARTING Node **/
StartingNode::StartingNode() { next = new EndingNode; }
StartingNode::~StartingNode() { delete next; }

Node* StartingNode::insert(Data* data) {
    next = next->insert(data);
    return this;
}

Data* StartingNode::getData(void) {
    return next->getData();
    /// Nodeen nach auslesen der Daten wieder löschen ...
    Node* old = next;
    next = next->getNext();
    delete old;
}

void StartingNode::releaseLastNode(void) {
    Node* old = next;
    next = next->getNext();
    delete old;
}

unsigned short StartingNode::getNodeCount(void) { return next->getNodeCount(); }

Node* StartingNode::getNext(void) { return next->getNext(); }

/** CLASS ENDING Node **/
EndingNode::EndingNode() {}
EndingNode::~EndingNode() {}

Node* EndingNode::insert(Data* data) {
    DataNode* dataNode = new DataNode(data, this);
    return dataNode;
}

unsigned short EndingNode::getNodeCount(void) { return 0; }

Data* EndingNode::getData(void) { return NULL; }

Node* EndingNode::getNext(void) { return NULL; }

/** CLASS DATA Node **/

unsigned short DataNode::NodeCount = 0;

DataNode::DataNode(Data* _data, Node* _Node) {
    data = _data;
    next = _Node;

    NodeCount++;
}

DataNode::~DataNode() {
    delete data;
    delete next;

    DataNode::NodeCount--;
}

Node* DataNode::insert(Data* data) {
    //DataNode* dataNode = new DataNode(data, this);
    //return dataNode;
    next = next->insert(data);
    return this;
}

Data* DataNode::getData(void) { return data; }

Node* DataNode::getNext(void) { return next; }
unsigned short DataNode::getNodeCount(void) { return NodeCount; }
