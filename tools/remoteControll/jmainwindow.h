#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_jmainwindow.h"

#include "jjoystick.h"
#include "jvector.h"
#include "jbotsimulator.h"
#include "jtcpmanager.h"

class JMainWindow : public QMainWindow
{
        Q_OBJECT
        
    public:
        JMainWindow(QWidget *parent = 0);
        ~JMainWindow();

    public slots:
        void onButtonChanged(int num, bool status);
        void onAxisChanged(int num, float value);

private slots:
        void on_buttonConnect_clicked();
        void on_buttonDisconnect_clicked();
        void connect_enabled(bool status);

private:
        Ui::JMainWindow *ui;

        JJoystick *stick;

        JBotSimulator *simu;

        JTcpManager *tcpManager;
};

#endif // MAINWINDOW_H
