/********************************************************************************
** Form generated from reading UI file 'enginewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENGINEWIDGET_H
#define UI_ENGINEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_engineWidget
{
public:

    void setupUi(QWidget *engineWidget)
    {
        if (engineWidget->objectName().isEmpty())
            engineWidget->setObjectName(QStringLiteral("engineWidget"));
        engineWidget->resize(640, 480);

        retranslateUi(engineWidget);

        QMetaObject::connectSlotsByName(engineWidget);
    } // setupUi

    void retranslateUi(QWidget *engineWidget)
    {
        engineWidget->setWindowTitle(QApplication::translate("engineWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class engineWidget: public Ui_engineWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENGINEWIDGET_H
