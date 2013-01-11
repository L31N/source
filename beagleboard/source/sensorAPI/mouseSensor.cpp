
// mouseSensor.cpp

#include "mouseSensor.h"

MouseSensor::MouseSensor(std::string _ipcName, Side _side) {
    ipcName = _ipcName;
    side = _side;

    ipcRCon = new ipcReceivingConnection(ipcName);

    nullVector.setX(0.0);
    nullVector.setY(0.0);

    relativeVector.setX(0.0);
    relativeVector.setY(0.0);
}

MouseSensor::~MouseSensor() {
    delete ipcRCon;
}

Vector MouseSensor::getNullVector() {
    if (ipcRCon->checkForNewData()) {
        Data* data = ipcRCon->readDataFromBuffer();
        std::string tmpStr = data->getData();
        // extract x

        // extract y

        nullVector.setX()
        //nullVector(data->getData());
    }
    return nullVector;
}
Vector MouseSensor::getRelativeVector() {
    relativeVector = this->getNullVector() - relativeVector;
    return relativeVector;
}
