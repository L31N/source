/********************************************************************************
** Form generated from reading UI file 'jmainwindow.ui'
**
** Created: Sat Jun 8 14:11:04 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JMAINWINDOW_H
#define UI_JMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "jdirectionalpad.h"
#include "jfieldview.h"

QT_BEGIN_NAMESPACE

class Ui_JMainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    JFieldView *fieldView;
    QHBoxLayout *horizontalLayout;
    JDirectionalPad *directionalPad1;
    JDirectionalPad *directionalPad2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *editAdress;
    QPushButton *buttonConnect;
    QPushButton *buttonDisconnect;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *JMainWindow)
    {
        if (JMainWindow->objectName().isEmpty())
            JMainWindow->setObjectName(QString::fromUtf8("JMainWindow"));
        JMainWindow->resize(435, 408);
        centralWidget = new QWidget(JMainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        fieldView = new JFieldView(centralWidget);
        fieldView->setObjectName(QString::fromUtf8("fieldView"));

        verticalLayout->addWidget(fieldView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        directionalPad1 = new JDirectionalPad(centralWidget);
        directionalPad1->setObjectName(QString::fromUtf8("directionalPad1"));

        horizontalLayout->addWidget(directionalPad1);

        directionalPad2 = new JDirectionalPad(centralWidget);
        directionalPad2->setObjectName(QString::fromUtf8("directionalPad2"));

        horizontalLayout->addWidget(directionalPad2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        editAdress = new QLineEdit(centralWidget);
        editAdress->setObjectName(QString::fromUtf8("editAdress"));

        horizontalLayout_2->addWidget(editAdress);

        buttonConnect = new QPushButton(centralWidget);
        buttonConnect->setObjectName(QString::fromUtf8("buttonConnect"));

        horizontalLayout_2->addWidget(buttonConnect);

        buttonDisconnect = new QPushButton(centralWidget);
        buttonDisconnect->setObjectName(QString::fromUtf8("buttonDisconnect"));

        horizontalLayout_2->addWidget(buttonDisconnect);


        verticalLayout->addLayout(horizontalLayout_2);

        JMainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(JMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        JMainWindow->setStatusBar(statusBar);

        retranslateUi(JMainWindow);

        QMetaObject::connectSlotsByName(JMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *JMainWindow)
    {
        JMainWindow->setWindowTitle(QApplication::translate("JMainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        buttonConnect->setText(QApplication::translate("JMainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        buttonDisconnect->setText(QApplication::translate("JMainWindow", "Disconnect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class JMainWindow: public Ui_JMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JMAINWINDOW_H
