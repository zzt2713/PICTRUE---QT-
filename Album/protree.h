#ifndef PROTREE_H
#define PROTREE_H
#include "Logger.h"
#include <QDialog>
#include <QTreeWidget>
namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    QTreeWidget *GetTreeWidget();
    ~ProTree();

private:
    Ui::ProTree *ui;

public slots:
    void AddProToTree(const QString &name,const QString &paht);
};

#endif // PROTREE_H
