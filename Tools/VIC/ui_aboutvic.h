/********************************************************************************
** Form generated from reading UI file 'aboutvic.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTVIC_H
#define UI_ABOUTVIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_AboutVIC
{
public:

    void setupUi(QDialog *AboutVIC)
    {
        if (AboutVIC->objectName().isEmpty())
            AboutVIC->setObjectName(QString::fromUtf8("AboutVIC"));
        AboutVIC->resize(500, 400);
        AboutVIC->setStyleSheet(QString::fromUtf8("background-image: url(:/images/About_Dialog.png);"));

        retranslateUi(AboutVIC);

        QMetaObject::connectSlotsByName(AboutVIC);
    } // setupUi

    void retranslateUi(QDialog *AboutVIC)
    {
        AboutVIC->setWindowTitle(QCoreApplication::translate("AboutVIC", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutVIC: public Ui_AboutVIC {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTVIC_H
