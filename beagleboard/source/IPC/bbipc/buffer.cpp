
#include "buffer.h"

/** CLASS BUFFER **/
Buffer::Buffer(unsigned short _maxElements) : maxElements(_maxElements) {}
Buffer::~Buffer() {}

void Buffer::insert(Data* data) {
    vector.push_back(*data);
    /// aeltestes element loeschen, falls maxElements erreicht
    //if (vector.size() > size_t(maxElements)) vector.erase(vector.begin());
    return;
}

Data* Buffer::getLastData(void) {
    /// erstes element im container auslesen und anschliessend löschen
    Data* data = &(vector.front());
    vector.erase(vector.begin());
    return data;
}

/** CLASS DATA **/
Data::Data(std::string _data, short _senderID) {
    data = _data;
    senderID = _senderID;
}

Data::~Data() {}

std::string Data::getData(void) { return data; }
short Data::getSenderID(void) { return senderID; }
