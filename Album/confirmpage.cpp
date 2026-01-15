#include "confirmpage.h"
#include "ui_confirmpage.h"

conFirmPage::conFirmPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::conFirmPage)
{
    ui->setupUi(this);
}

conFirmPage::~conFirmPage()
{
    delete ui;
}
