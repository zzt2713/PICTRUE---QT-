#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QProgressDialog>
#include <QTreeWidget>
#include <QWidget>
#include "Logger.h"
#include "protreethread.h"
#include "projecthistorymanager.h"
#include "opentreethread.h"
#include "protreeitem.h"

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString &name,const QString &path, bool saveToHistory = true);
    void loadRecentProjects();
    void loadProjectFiles(const QString &projectPath);
    void LoadProjectFilesImmediately(ProTreeItem* projectItem);
private:
    QSet<QString> _set_path;

    QTreeWidgetItem *_right_btn_item;
    QTreeWidgetItem *_active_item;
    QTreeWidgetItem *_selected_item;

    QAction *_action_import;
    QAction *_action_setstart;
    QAction *_action_closepro;
    QAction *_action_slideshow;

    QProgressDialog *_dialog_progress;
    QProgressDialog *_dialog_progressdlg;
    std::shared_ptr<ProTreeThread> _thread;
    std::shared_ptr<ProTreeThread> _thread_create;
    std::shared_ptr<OpenTreeThread> _thread_open;

    ProjectHistoryManager* _historyManager;

private slots:
    void SlotItemPressed(QTreeWidgetItem *item,int column);

    // 项目操作槽函数
    void SlotImport();
    void SlotSetActive();
    void SlotclosePro();


    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();

    void SlotUpOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCancelOPenProgress();

public slots:
    void SlotOpenPro(const QString &path);

signals:
    void SigCancelProgress();

};

#endif // PROTREEWIDGET_H
