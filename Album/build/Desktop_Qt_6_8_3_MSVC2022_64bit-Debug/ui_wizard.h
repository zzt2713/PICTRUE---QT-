/********************************************************************************
** Form generated from reading UI file 'wizard.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZARD_H
#define UI_WIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWizard>
#include "confirmpage.h"
#include "prosetpage.h"

QT_BEGIN_NAMESPACE

class Ui_Wizard
{
public:
    proSetPage *wizardPage1;
    conFirmPage *wizardPage2;

    void setupUi(QWizard *Wizard)
    {
        if (Wizard->objectName().isEmpty())
            Wizard->setObjectName("Wizard");
        Wizard->resize(400, 300);
        wizardPage1 = new proSetPage();
        wizardPage1->setObjectName("wizardPage1");
        Wizard->addPage(wizardPage1);
        wizardPage2 = new conFirmPage();
        wizardPage2->setObjectName("wizardPage2");
        Wizard->addPage(wizardPage2);

        retranslateUi(Wizard);

        QMetaObject::connectSlotsByName(Wizard);
    } // setupUi

    void retranslateUi(QWizard *Wizard)
    {
        Wizard->setWindowTitle(QCoreApplication::translate("Wizard", "Wizard", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Wizard: public Ui_Wizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARD_H
