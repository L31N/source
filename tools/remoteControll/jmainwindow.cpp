#include "jmainwindow.h"

JMainWindow::JMainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui = new Ui::JMainWindow;
    ui->setupUi(this);

    stick = new JJoystick(this);
    connect(stick, SIGNAL(statusChanged(QString)), ui->statusBar, SLOT(showMessage(QString)));
    connect(stick, SIGNAL(buttonChanged(int,bool)), this, SLOT(onButtonChanged(int,bool)));
    connect(stick, SIGNAL(axisChanged(int,float)), this, SLOT(onAxisChanged(int,float)));
    stick->check();

    simu = new JBotSimulator(this);

    connect(simu, SIGNAL(positionChanged(int,JVector)), ui->fieldView, SLOT(updatePos(int,JVector)));

    tcpManager = new JTcpManager(this);
    connect(tcpManager, SIGNAL(newMessage(QString)), this->ui->statusBar, SLOT(showMessage(QString)));
    connect(tcpManager, SIGNAL(newStatus(bool)), this, SLOT(connect_enabled(bool)));

}

JMainWindow::~JMainWindow()
{
    stick->quit();
    delete ui;
}

void JMainWindow::onButtonChanged(int num, bool status)
{
    //qDebug() << "Button changed: " << num << " : " << status;

    if(num == 11 && status == false) simu->setRotation(0);

}

void JMainWindow::onAxisChanged(int num, float value)
{
    switch(num)
    {
        case 0:
            ui->directionalPad1->setX(value);
            tcpManager->updateX(value);
            simu->setMovementX(value);
            break;
        case 1:
            ui->directionalPad1->setY(value);
            tcpManager->updateY(value);
            simu->setMovementY(-value);
            break;
        case 3:
            ui->directionalPad2->setX(value);
            tcpManager->updateRotation(value);
            simu->setRotationSpeed(value*255);
            break;
        case 4:
            ui->directionalPad2->setY(value);
            break;
        default:
            break;
    }
}

void JMainWindow::on_buttonConnect_clicked()
{
    ui->statusBar->showMessage("Starting connection to server");
    ui->buttonConnect->setEnabled(false);
    tcpManager->connectServer(ui->editAdress->text());
}

void JMainWindow::on_buttonDisconnect_clicked()
{
    tcpManager->disconnectServer();
}

void JMainWindow::connect_enabled(bool status)
{
    ui->buttonConnect->setEnabled(status);
}
