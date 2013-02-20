
// mouseSensor.cpp

#include "mouseSensor.h"

MouseSensor::MouseSensor(std::string _ipcName) {
    ipcName = _ipcName;
    ipcRCon = new ipcReceivingConnection(ipcName);

}

MouseSensor::~MouseSensor() {
    delete ipcRCon;
}

Vector MouseSensor::getRelativeVector() {
    this->updateData();
    return relativeVector;
}

Vector MouseSensor::getPositionVector() {
    this->updateData();
    return positionVector;
}

Vector MouseSensor::getRelativeDirVector() {
    this->updateData();
    return relDirVector;
}

Vector MouseSensor::getAbsoluteDirVector() {
    this->updateData();
    return absDirVector;
}

/*Angle MouseSensor::getAbsoluteAngle() {
    return Angle(absDirVector.getAngle());
}*/

/*Angle MouseSensor::getRelativeAngle() {
    return Angle(relDirVector.getAngle());
}*/


void MouseSensor::updateData() {
     if (ipcRCon->checkForNewData()) {
        Data* data = ipcRCon->readDataFromBuffer();
        std::string tmpStr = data->getData();

        relativeVector = Vector(tmpStr.substr(0, sizeof(Vector)));
        relDirVector = Vector(tmpStr.substr(sizeof(Vector), sizeof(Vector)));

        // determine the absolute values ...
        positionVector += relativeVector;
        absDirVector.setAngle(absDirVector, relDirVector.getAngle());

        delete data;

    }
    return;
}
