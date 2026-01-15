/********************************************************************************
** Form generated from reading UI file 'confirmpage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRMPAGE_H
#define UI_CONFIRMPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_conFirmPage
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QWizardPage *conFirmPage)
    {
        if (conFirmPage->objectName().isEmpty())
            conFirmPage->setObjectName("conFirmPage");
        conFirmPage->resize(400, 300);
        gridLayout = new QGridLayout(conFirmPage);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(conFirmPage);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        verticalLayout->addWidget(label);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(conFirmPage);

        QMetaObject::connectSlotsByName(conFirmPage);
    } // setupUi

    void retranslateUi(QWizardPage *conFirmPage)
    {
        conFirmPage->setWindowTitle(QCoreApplication::translate("conFirmPage", "WizardPage", nullptr));
        label->setText(QCoreApplication::translate("conFirmPage", "\347\202\271\345\207\273\345\256\214\346\210\220\357\274\214\345\256\214\346\210\220\345\210\233\345\273\272\357\274\201\346\210\226\347\202\271\345\207\273\345\217\226\346\266\210\357\274\214\345\217\226\346\266\210\346\234\254\346\254\241\345\210\233\345\273\272\343\200\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class conFirmPage: public Ui_conFirmPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMPAGE_H
