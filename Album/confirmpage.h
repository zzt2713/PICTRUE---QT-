#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H
#include "Logger.h"
#include <QWizardPage>

namespace Ui {
class conFirmPage;
}

class conFirmPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit conFirmPage(QWidget *parent = nullptr);
    ~conFirmPage();

private:
    Ui::conFirmPage *ui;
};

#endif // CONFIRMPAGE_H
