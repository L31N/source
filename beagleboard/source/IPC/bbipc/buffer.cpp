
#include "buffer.h"

#include "bbipc.h"

/** CLASS BUFFER **/
Buffer::Buffer(unsigned short _maxElements) : maxElements(_maxElements) {}
Buffer::~Buffer() {}

void Buffer::insert(Data* data) {
    vector.push_back(*data);
    /// aeltestes element loeschen, falls maxElements erreicht
    if (vector.size() > size_t(maxElements)) vector.erase(vector.begin());
    return;
}

bool Buffer::checkForNewData() {
    if (vector.size() > 0) return true;
    else return false;
}

Data* Buffer::getLastData(void) {
    /// erstes element im container auslesen und anschliessend löschen
    //Data* data = &(vector.front());
    if (vector.size() > 0) {
        Data* data = new Data(vector.front());
        vector.erase(vector.begin());
        return data;
    }
    else return NULL;
}

/** CLASS DATA **/
Data::Data(std::string _data, short _senderID, HOST_TYPE _host) {
    data = _data;
    senderID = _senderID;
    host = _host;
}

Data::~Data() {}

std::string Data::getData(void) { return data; }
short Data::getSenderID(void) { return senderID; }
HOST_TYPE Data::getHost(void) { return host; }
