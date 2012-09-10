
#include "buffer.h"

/** CLASS BUFFER **/
Buffer::Buffer(unsigned short _maxElements) { maxElements = _maxElements; }
Buffer::~Buffer() {}

void Buffer::insert(Data* data) {
    start->insert(data);

    /// To many elements exists ... delete the latest
    if (start->getKnotCount() > maxElements) start->releaseLastKnot();

    return;
}

Data* Buffer::getLastData(void) { return start->getData(); }

/** CLASS DATA **/
Data::Data() {}
Data::~Data() {}

/** CLASS STARTING KNOT **/
StartingKnot::StartingKnot() { next = new EndingKnot; }
StartingKnot::~StartingKnot() { delete next; }

Knot* StartingKnot::insert(Data* data) {
    next = next->insert(data);
    return this;
}

Data* StartingKnot::getData(void) {
    return next->getData();
    /// Knoten nach auslesen der Daten wieder löschen ...
    Knot* old = next;
    next = next->getNext();
    delete old;
}

void StartingKnot::releaseLastKnot(void) {
    Knot* old = next;
    next = next->getNext();
    delete old;
}

unsigned short StartingKnot::getKnotCount(void) { return next->getKnotCount(); }

Knot* StartingKnot::getNext(void) { return next->getNext(); }

/** CLASS ENDING KNOT **/
EndingKnot::EndingKnot() {}
EndingKnot::~EndingKnot() {}

Knot* EndingKnot::insert(Data* data) {
    DataKnot* dataKnot = new DataKnot(data, this);
    return dataKnot;
}

unsigned short EndingKnot::getKnotCount(void) { return 0; }

Data* EndingKnot::getData(void) { return NULL; }

Knot* EndingKnot::getNext(void) { return NULL; }

/** CLASS DATA KNOT **/
DataKnot::DataKnot(Data* _data, Knot* _knot) {
    data = _data;
    next = _knot;

    knotCount++;
}

DataKnot::~DataKnot() {
    delete data;
    delete next;

    knotCount--;
}

Knot* DataKnot::insert(Data* data) {
    //DataKnot* dataKnot = new DataKnot(data, this);
    //return dataKnot;
    next = next->insert(data);
    return this;
}

Data* DataKnot::getData(void) { return data; }

Knot* DataKnot::getNext(void) { return next; }
unsigned short DataKnot::getKnotCount(void) { return knotCount; }
