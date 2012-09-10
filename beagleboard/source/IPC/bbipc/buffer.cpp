
#include "buffer.h"

/** CLASS BUFFER **/
Buffer::Buffer(unsigned short _maxElements) { maxElements = _maxElements; }
Buffer::~Buffer() {}

void Buffer::insert(Data* data) {
    start->insert(data);
    return;
}

Data* Buffer::getLastData(void) { return start->getData(void); }

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
    return next->getData(void);
    /// Knoten nach auslesen der Daten wieder löschen ...
    Knot* old = next;
    next = next->getNext();
    delete old;
}

/** CLASS ENDING KNOT **/
EndingKnot::EndingKnot() {}
EndingKnot::~EndingKnot() {}

Knot* EndingKnot::insert(Data* data) {
    DataKnot* dataKnot = new DataKnot(data, this);
    return dataKnot;
}

/** CLASS DATA KNOT **/
DataKnot::DataKnot(Data* _data, Knot* _knot) {
    data = _data;
    next = _knot;
}

DataKnot::~DataKnot() {
    delete data;
    delete next;
}

Knot* DataKnot::insert(Data* data) {
    //DataKnot* dataKnot = new DataKnot(data, this);
    //return dataKnot;
    next = next->insert(data);
    return this;
}

Data* DataKnot::getData(void) { return data; }

Knot* DataKnot::getNext(void) { return next; }
