/********************************************************************************
** Form generated from reading UI file 'removepro.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOVEPRO_H
#define UI_REMOVEPRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemovePro
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer_3;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *RemovePro)
    {
        if (RemovePro->objectName().isEmpty())
            RemovePro->setObjectName("RemovePro");
        RemovePro->resize(400, 150);
        verticalLayout = new QVBoxLayout(RemovePro);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(RemovePro);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(18);
        font.setBold(false);
        label->setFont(font);

        verticalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        checkBox = new QCheckBox(RemovePro);
        checkBox->setObjectName("checkBox");

        verticalLayout->addWidget(checkBox);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        buttonBox = new QDialogButtonBox(RemovePro);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(RemovePro);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, RemovePro, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, RemovePro, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(RemovePro);
    } // setupUi

    void retranslateUi(QDialog *RemovePro)
    {
        RemovePro->setWindowTitle(QCoreApplication::translate("RemovePro", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("RemovePro", "\346\230\257\345\220\246\347\241\256\350\256\244\344\273\216\351\241\271\347\233\256\344\270\255\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        checkBox->setText(QCoreApplication::translate("RemovePro", "\345\220\214\346\227\266\345\210\240\351\231\244\346\234\254\345\234\260\351\241\271\347\233\256\346\226\207\344\273\266\345\244\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemovePro: public Ui_RemovePro {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOVEPRO_H
