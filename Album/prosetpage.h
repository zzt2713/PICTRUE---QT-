#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>
#include "Logger.h"
namespace Ui {
class proSetPage;
}

class proSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit proSetPage(QWidget *parent = nullptr);
    ~proSetPage();
    void GetProSettings(QString &name,QString &path);

protected:
    virtual bool isComplete() const;

private slots:
    void on_pushButton_clicked();

private:
    Ui::proSetPage *ui;
};

#endif // PROSETPAGE_H
