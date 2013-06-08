#ifndef JTCPMANAGER_H
#define JTCPMANAGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDataStream>

class JTcpManager : public QObject
{
    Q_OBJECT
    public:
        JTcpManager(QObject *parent = 0);

    private:
        QTcpSocket *socket;
        bool connected;

        qint16 Xaxis;
        qint16 Yaxis;
        qint16 Rotation;

        void sendValues();

    signals:
        void newMessage(QString message);
        void newStatus(bool enabled);

    public slots:
        void connectServer(QString ip);
        void disconnectServer();
        void updateX(float value);
        void updateY(float value);
        void updateRotation(float value);

    private slots:
        void connectSuccessfull();
        void connectFailure(QAbstractSocket::SocketError);
        void disconnectSuccessfull();
    
};

#endif // JTCPMANAGER_H
