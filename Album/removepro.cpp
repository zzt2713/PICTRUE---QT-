#include "removepro.h"
#include "ui_removepro.h"

RemovePro::RemovePro(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemovePro)
{
    ui->setupUi(this);

    // 对话框样式
    this->setStyleSheet(
        "QDialog {"
        "    background-color: #FFFFFF;"
        "    border: 1px solid #E2E8F0;"
        "    border-radius: 8px;"
        "}"
    );
    // 删除提示字样式
    ui->label->setStyleSheet(
        "color: #721C24;"
        "background-color: #F8D7DA;"
        "border: 1px solid #F5C6CB;"
        "border-radius: 6px;"
        "padding: 12px 15px;"
        "font-size: 14px;"
        "font-weight: 500;"
        );

    // 设置 checkBox 样式
    ui->checkBox->setStyleSheet(
        "QCheckBox {"
        "    color: #212529;"
        "    font-size: 13px;"
        "    spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 16px;"
        "    height: 16px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "    border: 2px solid #6C757D;"
        "    background-color: white;"
        "    border-radius: 3px;"
        "}"
        "QCheckBox::indicator:checked {"
        "    border: 2px solid #DC3545;"
        "    background-color: #DC3545;"
        "    border-radius: 3px;"
        "}"
        );

}

RemovePro::~RemovePro()
{
    delete ui;
}

bool RemovePro::isRemoved()
{
    return ui->checkBox->isChecked();
}
