/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionVirtual_IC;
    QAction *actionExit;
    QAction *actionVersion;
    QAction *actionTerminal;
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QPushButton *On_Off_One;
    QPushButton *On_Off_Two;
    QPushButton *On_Off_Three;
    QPushButton *On_Off_Four;
    QPushButton *On_Off_Five;
    QPushButton *On_Off_Six;
    QPushButton *On_Off_Seven;
    QPushButton *On_Off_Seven_2;
    QLineEdit *lineEdit;
    QPushButton *send_Button;
    QLineEdit *lineEdit_2;
    QMenuBar *menuBar;
    QMenu *menuAbout;
    QMenu *menuAbout_2;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1200, 700);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8("background-image: url(:/images/vic_ui_3.png);\n"
"background-color: rgb(136, 138, 133);\n"
"selection-background-color: rgb(85, 87, 83);\n"
"color: rgb(255, 255, 255);"));
        actionVirtual_IC = new QAction(MainWindow);
        actionVirtual_IC->setObjectName(QString::fromUtf8("actionVirtual_IC"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionVersion = new QAction(MainWindow);
        actionVersion->setObjectName(QString::fromUtf8("actionVersion"));
        actionTerminal = new QAction(MainWindow);
        actionTerminal->setObjectName(QString::fromUtf8("actionTerminal"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 1200, 700));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color:transparent;"));
        graphicsView->setFrameShape(QFrame::NoFrame);
        On_Off_One = new QPushButton(centralWidget);
        On_Off_One->setObjectName(QString::fromUtf8("On_Off_One"));
        On_Off_One->setGeometry(QRect(610, 220, 61, 41));
        On_Off_One->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Two = new QPushButton(centralWidget);
        On_Off_Two->setObjectName(QString::fromUtf8("On_Off_Two"));
        On_Off_Two->setGeometry(QRect(750, 220, 61, 41));
        On_Off_Two->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Three = new QPushButton(centralWidget);
        On_Off_Three->setObjectName(QString::fromUtf8("On_Off_Three"));
        On_Off_Three->setGeometry(QRect(890, 220, 61, 41));
        On_Off_Three->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Four = new QPushButton(centralWidget);
        On_Off_Four->setObjectName(QString::fromUtf8("On_Off_Four"));
        On_Off_Four->setGeometry(QRect(1030, 220, 61, 41));
        On_Off_Four->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Five = new QPushButton(centralWidget);
        On_Off_Five->setObjectName(QString::fromUtf8("On_Off_Five"));
        On_Off_Five->setGeometry(QRect(610, 440, 61, 41));
        On_Off_Five->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Six = new QPushButton(centralWidget);
        On_Off_Six->setObjectName(QString::fromUtf8("On_Off_Six"));
        On_Off_Six->setGeometry(QRect(750, 440, 61, 41));
        On_Off_Six->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Seven = new QPushButton(centralWidget);
        On_Off_Seven->setObjectName(QString::fromUtf8("On_Off_Seven"));
        On_Off_Seven->setGeometry(QRect(890, 440, 61, 41));
        On_Off_Seven->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        On_Off_Seven_2 = new QPushButton(centralWidget);
        On_Off_Seven_2->setObjectName(QString::fromUtf8("On_Off_Seven_2"));
        On_Off_Seven_2->setGeometry(QRect(1030, 440, 61, 41));
        On_Off_Seven_2->setStyleSheet(QString::fromUtf8("color: rgb(238, 238, 236);"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(230, 510, 291, 25));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"selection-background-color: rgb(85, 87, 83);\n"
"color: rgb(255, 255, 255);\n"
"border-color: rgb(0, 0, 0);"));
        send_Button = new QPushButton(centralWidget);
        send_Button->setObjectName(QString::fromUtf8("send_Button"));
        send_Button->setGeometry(QRect(40, 510, 89, 25));
        send_Button->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(150, 510, 71, 25));
        lineEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"selection-background-color: rgb(85, 87, 83);\n"
"color: rgb(255, 255, 255);\n"
"border-color: rgb(0, 0, 0);"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 22));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuAbout_2 = new QMenu(menuBar);
        menuAbout_2->setObjectName(QString::fromUtf8("menuAbout_2"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuAbout->menuAction());
        menuBar->addAction(menuAbout_2->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuAbout->addAction(actionTerminal);
        menuAbout->addAction(actionExit);
        menuAbout_2->addAction(actionVirtual_IC);
        menuHelp->addAction(actionVersion);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Virtual IC", nullptr));
        actionVirtual_IC->setText(QCoreApplication::translate("MainWindow", "Virtual IC", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionVersion->setText(QCoreApplication::translate("MainWindow", "Version", nullptr));
        actionTerminal->setText(QCoreApplication::translate("MainWindow", "Terminal", nullptr));
#if QT_CONFIG(statustip)
        graphicsView->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        On_Off_One->setText(QCoreApplication::translate("MainWindow", "ON/Off", nullptr));
        On_Off_Two->setText(QCoreApplication::translate("MainWindow", "ON/Off", nullptr));
        On_Off_Three->setText(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
        On_Off_Four->setText(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
        On_Off_Five->setText(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
        On_Off_Six->setText(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
        On_Off_Seven->setText(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
        On_Off_Seven_2->setText(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit->setToolTip(QCoreApplication::translate("MainWindow", "CAN Frame Data", nullptr));
#endif // QT_CONFIG(tooltip)
        send_Button->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_2->setToolTip(QCoreApplication::translate("MainWindow", "Arbitration ID", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEdit_2->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(accessibility)
        lineEdit_2->setAccessibleDescription(QString());
#endif // QT_CONFIG(accessibility)
        lineEdit_2->setPlaceholderText(QString());
        menuAbout->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuAbout_2->setTitle(QCoreApplication::translate("MainWindow", "About", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
