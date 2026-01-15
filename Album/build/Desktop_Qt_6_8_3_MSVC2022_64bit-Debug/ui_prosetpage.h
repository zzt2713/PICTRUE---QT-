/********************************************************************************
** Form generated from reading UI file 'prosetpage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROSETPAGE_H
#define UI_PROSETPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_proSetPage
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QPushButton *pushButton;
    QLabel *tips;

    void setupUi(QWizardPage *proSetPage)
    {
        if (proSetPage->objectName().isEmpty())
            proSetPage->setObjectName("proSetPage");
        proSetPage->setEnabled(true);
        proSetPage->resize(441, 100);
        gridLayout_3 = new QGridLayout(proSetPage);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(5, 5, 5, 5);
        label = new QLabel(proSetPage);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(proSetPage);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        lineEdit_2 = new QLineEdit(proSetPage);
        lineEdit_2->setObjectName("lineEdit_2");

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        label_2 = new QLabel(proSetPage);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pushButton = new QPushButton(proSetPage);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 1, 2, 1, 1);

        tips = new QLabel(proSetPage);
        tips->setObjectName("tips");
        QFont font;
        font.setPointSize(13);
        tips->setFont(font);

        gridLayout->addWidget(tips, 2, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(proSetPage);

        QMetaObject::connectSlotsByName(proSetPage);
    } // setupUi

    void retranslateUi(QWizardPage *proSetPage)
    {
        proSetPage->setWindowTitle(QCoreApplication::translate("proSetPage", "WizardPage", nullptr));
        label->setText(QCoreApplication::translate("proSetPage", "Name", nullptr));
        label_2->setText(QCoreApplication::translate("proSetPage", "Path", nullptr));
        pushButton->setText(QCoreApplication::translate("proSetPage", "\346\265\217\350\247\210", nullptr));
        tips->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class proSetPage: public Ui_proSetPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROSETPAGE_H
