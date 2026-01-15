#ifndef REMOVEPRO_H
#define REMOVEPRO_H

#include <QDialog>

namespace Ui {
class RemovePro;
}

class RemovePro : public QDialog
{
    Q_OBJECT

public:
    explicit RemovePro(QWidget *parent = nullptr);
    ~RemovePro();
    bool isRemoved();


private:
    Ui::RemovePro *ui;
};

#endif // REMOVEPRO_H
