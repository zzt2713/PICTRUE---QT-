/********************************************************************************
** Form generated from reading UI file 'protree.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROTREE_H
#define UI_PROTREE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "protreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_ProTree
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelPro;
    ProTreeWidget *treeWidget;

    void setupUi(QDialog *ProTree)
    {
        if (ProTree->objectName().isEmpty())
            ProTree->setObjectName("ProTree");
        ProTree->resize(400, 300);
        gridLayout = new QGridLayout(ProTree);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        labelPro = new QLabel(ProTree);
        labelPro->setObjectName("labelPro");
        QFont font;
        font.setPointSize(13);
        labelPro->setFont(font);

        verticalLayout->addWidget(labelPro);

        treeWidget = new ProTreeWidget(ProTree);
        treeWidget->headerItem()->setText(0, QString());
        treeWidget->setObjectName("treeWidget");

        verticalLayout->addWidget(treeWidget);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 20);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(ProTree);

        QMetaObject::connectSlotsByName(ProTree);
    } // setupUi

    void retranslateUi(QDialog *ProTree)
    {
        ProTree->setWindowTitle(QCoreApplication::translate("ProTree", "Dialog", nullptr));
        labelPro->setText(QCoreApplication::translate("ProTree", "\351\241\271\347\233\256\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProTree: public Ui_ProTree {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROTREE_H
