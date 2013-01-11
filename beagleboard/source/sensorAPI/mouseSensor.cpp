
// mouseSensor.cpp

#include "mouseSensor.h"

MouseSensor::MouseSensor(std::string _ipcName) {
    ipcName = _ipcName;
    ipcRCon = new ipcReceivingConnection(ipcName);

}

MouseSensor::~MouseSensor() {
    delete ipcRCon;
}

Vector MouseSensor::getRelativeAngle() {
    this->updateData();
    return relativeVector;
}

Vector MouseSensor::getRelativeDirVector() {
    this->updateData();
    return relDirVector();
}

Vector MouseSensor::getPositionVector() {
    this->updateData();
    return positionVector;
}

Vector MouseSensor::getAbsoluteDirVector() {
    this->updateData();
    return absDirVector;
}

Angle MouseSensor::getAbsoluteAngle() {

}

Angle MouseSensor::getRelativeAngle() {

}


void MouseSensor::updateData() {
     if (ipcRCon->checkForNewData()) {
        Data* data = ipcRCon->readDataFromBuffer();
        std::string tmpStr = data->getData();
        char* cRelVect = new char[sizeof(Vector)];
        char* cDirVect = new char[sizeof(Vector)];
        cRelVect = (char*) tmpStr.substr(0, sizeof(Vector)).c_str();
        cDirVect = (char*) tmpStr.substr(sizeof(Vector), sizeof(Vector)).c_str();

        Vector* RelVect = (Vector*)cRelVect;
        Vector* DirVect = (Vector*)cDirVect;

        relativeVector(*RelVect);
        relDirVector(*DirVect);

        // determine the absolute values ...
        positionVector += relativeVector;
        absDirVector +=

        delete data;
        delete cRelVect;
        delete cDirVect;

    }
    return;
}
