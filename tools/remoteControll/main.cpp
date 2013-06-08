#include <QApplication>

#include "jmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JMainWindow w;
    w.show();
    
    return a.exec();
}
