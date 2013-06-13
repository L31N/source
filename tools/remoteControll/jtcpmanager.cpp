#include "jtcpmanager.h"

JTcpManager::JTcpManager(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    Xaxis = 0;
    Yaxis = 0;
    Rotation = 0;

    connect(socket, SIGNAL(connected()), this, SLOT(connectSuccessfull()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectSuccessfull()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectFailure(QAbstractSocket::SocketError)));

    connected = false;
}

void JTcpManager::sendValues()
{
    if(connected)
    {
        QDataStream stream(socket);
        stream << Xaxis;
        stream << Yaxis;
        stream << Rotation;
        stream << char(1);
    }
}

void JTcpManager::connectServer(QString ip)
{
    if(!connected)
    {
        emit newStatus(false);
        socket->connectToHost(ip, 4242);
    }
}

void JTcpManager::disconnectServer()
{
    if(connected)
    {
        socket->disconnectFromHost();
    }
}

void JTcpManager::updateX(float value)
{
    Xaxis = value*255.0;
    sendValues();
}

void JTcpManager::updateY(float value)
{
    Yaxis = value*255.0;
    sendValues();
}

void JTcpManager::updateRotation(float value)
{
    Rotation = value*255.0;
    sendValues();
}

void JTcpManager::connectSuccessfull()
{
    connected = true;
    emit newMessage("Connection Successfull!");
    emit newStatus(false);
}

void JTcpManager::connectFailure(QAbstractSocket::SocketError)
{
    emit newMessage("Connect error: " + socket->errorString());
    emit newStatus(true);
}

void JTcpManager::disconnectSuccessfull()
{
    connected = false;
    emit newMessage("Connection closed successfully!");
    emit newStatus(true);
}
