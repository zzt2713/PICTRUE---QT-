#include "protreewidget.h"
#include <QDir>
#include "protreeitem.h"
#include "protreethread.h"
#include "removepro.h"
#include "const.h"
#include <QHeaderView>
#include <QGuiApplication>
#include <QMenu>
#include <QFileDialog>
#include <QFont>
#include <QDebug>
#include "opentreethread.h"
#include <QQueue>

ProTreeWidget::ProTreeWidget(QWidget *parent):QTreeWidget(parent),_right_btn_item(nullptr),
    _active_item(nullptr),_dialog_progress(nullptr),_selected_item(nullptr),_dialog_progressdlg(nullptr),
    _thread_create(nullptr),_thread_open(nullptr)
{
    this->header()->hide();
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);

    _action_import = new QAction(QIcon(":/icon/import.png"),tr("导入文件"), this);
    _action_setstart = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"),this);
    _action_closepro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图播放"),this);

    // 初始化历史管理器
    _historyManager = ProjectHistoryManager::instance();

    // 加载最近项目
    loadRecentProjects();

    //连接槽函数
    connect(_action_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(_action_setstart,&QAction::triggered, this,&ProTreeWidget::SlotSetActive);
    connect(_action_closepro,&QAction::triggered, this,&ProTreeWidget::SlotclosePro);
}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path, bool saveToHistory)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if(_set_path.find(file_path) != _set_path.end()){
        // 项目已存在，更新历史记录
        if (saveToHistory) {
            _historyManager->updateProjectTime(file_path);
        }
        return;
    }

    QDir pro_dir(file_path);
    if(!pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path);
        if(!enable){
            return;
        }
    }

    _set_path.insert(file_path);
    auto *item = new ProTreeItem(this,name,file_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,file_path);
    this->addTopLevelItem(item);

    // 保存到历史记录
    if (saveToHistory) {
        Logger::getInstance().log("正在保存历史记录...",INFO);
        _historyManager->addProject(name, file_path);
        Logger::getInstance().log("历史记录已保存至数据库!",INFO);
    }
}

void ProTreeWidget::loadRecentProjects()
{
    auto recentProjects = _historyManager->getRecentProjects(10);

    // 加载之前创建的项目
    for (const auto& project : recentProjects) {
        // 检查项目路径是否存在
        QDir proDir(project.path);
        if (proDir.exists()) {
            // **关键修改：确保项目已经被添加到树中**
            // 检查是否已经存在
            bool alreadyExists = false;
            for (int i = 0; i < this->topLevelItemCount(); ++i) {
                ProTreeItem* item = dynamic_cast<ProTreeItem*>(this->topLevelItem(i));
                if (item && item->GetPath() == project.path) {
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists) {
                // 添加项目根节点到树
                QFileInfo proInfo(project.path);
                QString proName = proInfo.fileName();
                QString proParentPath = proInfo.absolutePath();

                AddProToTree(proName, proParentPath, false);
            }

            // **关键：立即加载项目文件结构**
            // 找到项目根节点
            ProTreeItem* projectItem = nullptr;
            for (int i = 0; i < this->topLevelItemCount(); ++i) {
                ProTreeItem* item = dynamic_cast<ProTreeItem*>(this->topLevelItem(i));
                if (item && item->GetPath() == project.path) {
                    projectItem = item;
                    break;
                }
            }

            if (projectItem) {

                LoadProjectFilesImmediately(projectItem);
            }
        }
    }
    Logger::getInstance().log("加载之前打开的项目成功...", INFO);
}

void ProTreeWidget::LoadProjectFilesImmediately(ProTreeItem* projectItem)
{
    if (!projectItem) return;

    QString projectPath = projectItem->GetPath();
    QDir proDir(projectPath);

    if (!proDir.exists()) return;

    // 清空已有子项
    while (projectItem->childCount() > 0) {
        delete projectItem->takeChild(0);
    }

    // 同步加载文件和目录
    QQueue<QPair<QString, QTreeWidgetItem*>> dirQueue;
    dirQueue.enqueue(qMakePair(projectPath, static_cast<QTreeWidgetItem*>(projectItem)));

    while (!dirQueue.isEmpty()) {
        auto current = dirQueue.dequeue();
        QString currentPath = current.first;
        QTreeWidgetItem* parentItem = current.second;

        QDir dir(currentPath);
        dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        dir.setSorting(QDir::Name);

        QFileInfoList entries = dir.entryInfoList();
        for (const auto& fileInfo : entries) {
            if (fileInfo.isDir()) {
                auto* dirItem = new ProTreeItem(parentItem, fileInfo.fileName(),
                                                fileInfo.absoluteFilePath(),
                                                projectItem, TreeItemDir);
                dirItem->setData(0, Qt::DisplayRole, fileInfo.fileName());
                dirItem->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
                dirItem->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
                dirQueue.enqueue(qMakePair(fileInfo.absoluteFilePath(),
                                           static_cast<QTreeWidgetItem*>(dirItem)));
            }
        }

        for (const auto& fileInfo : entries) {
            if (fileInfo.isFile()) {
                QString suffix = fileInfo.suffix().toLower();
                if (suffix == "png" || suffix == "jpg" ||
                    suffix == "jpeg" || suffix == "gif") {

                    auto* fileItem = new ProTreeItem(parentItem, fileInfo.fileName(),
                                                     fileInfo.absoluteFilePath(),
                                                     projectItem, TreeItemPic);
                    fileItem->setData(0, Qt::DisplayRole, fileInfo.fileName());
                    fileItem->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
                    fileItem->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
                }
            }
        }
    }

    // 展开项目
    //projectItem->setExpanded(true);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        QMenu menu(this);
        auto itemtype = pressedItem->type();
        if(itemtype == TreeItemPro){
            _right_btn_item = dynamic_cast<ProTreeItem*>(pressedItem);
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_slideshow);
            menu.addAction(_action_closepro);
            menu.exec(QCursor::pos());
        }
    } else if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
        // 左键点击时更新项目时间戳
        auto* treeItem = dynamic_cast<ProTreeItem*>(pressedItem);
        if (treeItem) {
            _historyManager->updateProjectTime(treeItem->GetPath());
        }
    }
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_btn_item){
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);

    if(_active_item){
        _active_item->setFont(0,nullFont);
    }
    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0,nullFont);
    Logger::getInstance().log("项目"+ _right_btn_item->text(0).toStdString() + ",已设置为活动项目。");
    ProTreeItem* proItem = dynamic_cast<ProTreeItem*>(_active_item);
    if (proItem) {
        _historyManager->setActiveProject(proItem->GetPath());
    }
}

void ProTreeWidget::SlotclosePro()
{
    RemovePro re;
    auto res = re.exec();
    if(res != QDialog::Accepted){
        return ;
    }
    bool remove = re.isRemoved();

    auto index_right = this->indexOfTopLevelItem(_right_btn_item);
    auto * protreeitem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto * selecteditem = dynamic_cast<ProTreeItem*>(_selected_item);
    auto path = protreeitem->GetPath();
    _set_path.remove(path);
    if(remove){
        QDir dele(path);
        dele.removeRecursively();
        Logger::getInstance().log("项目"+ _right_btn_item->text(0).toStdString() + "删除成功",INFO);
    }

    if(protreeitem == _active_item){
        _active_item = nullptr;
    }

    delete this->takeTopLevelItem(index_right);
    _right_btn_item = nullptr;
    Logger::getInstance().log("关闭项目成功",INFO);
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(_set_path.find(path) != _set_path.end()){
        return;
    }
    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    QString proname = pro_dir.dirName();

    _thread_open = std::make_shared<OpenTreeThread>(path,file_count,this,nullptr);
    _thread_open->start();

    Logger::getInstance().log("文件:" + proname.toStdString() + "正在导入",INFO);

    _dialog_progressdlg = new QProgressDialog(this);
    connect(_thread_open.get(), &OpenTreeThread::SigUpdateProgress,
            this, &ProTreeWidget::SlotUpOpenProgress);

    connect(_thread_open.get(), &OpenTreeThread::SigFinishProgress, this,
            &ProTreeWidget::SlotFinishOpenProgress);

    _dialog_progressdlg->setWindowTitle("Please wait...");
    _dialog_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progressdlg->setRange(0, PROGRESS_MAX);
    _dialog_progressdlg->exec();
}

void ProTreeWidget::SlotImport()
{
    Logger::getInstance().log("正在导入文件夹...",INFO);
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setWindowTitle("选择导入的文件夹");
    QString path = "";

    if(!_right_btn_item){
        qDebug() << "right btn item is not" ;
        Logger::getInstance().log("右键按钮错误点击!",ERROR);
        return ;
    }

    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    dialog.setDirectory(path);

    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
    }

    if(fileNames.length() <= 0){
        return;
    }

    QString import_path = fileNames.at(0);
    int file_count = 0;
    _dialog_progress = new QProgressDialog(this);

    _thread = std::make_shared<ProTreeThread>(std::ref(import_path),std::ref(path),
                _right_btn_item,file_count,this,_right_btn_item,nullptr);

    connect(_thread.get(),&ProTreeThread::SigUpdateProgress,this,&ProTreeWidget::SlotUpdateProgress);
    connect(_thread.get(),&ProTreeThread::SigFinishProgress,this,&ProTreeWidget::SlotFinishProgress);
    connect(_dialog_progress,&QProgressDialog::canceled,this,&ProTreeWidget::SlotCancelProgress);
    connect(this,&ProTreeWidget::SigCancelProgress,_thread.get(),&ProTreeThread::SlotCancelProgress);

    _thread->start();

    _dialog_progress->setWindowTitle("Please wait...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0,PROGRESS_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_dialog_progress){
        return;
    }
    if(count >= PROGRESS_MAX){
        _dialog_progress->setValue(count % PROGRESS_MAX);
    }else{
        _dialog_progress->setValue(count % PROGRESS_MAX);
    }

}

void ProTreeWidget::SlotFinishProgress()
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
    Logger::getInstance().log("文件夹导入完成!",INFO);
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotUpOpenProgress(int count)
{
    if(!_dialog_progressdlg){
        return;
    }
    if(count >= PROGRESS_MAX){
        _dialog_progressdlg->setValue(count % PROGRESS_MAX);
    }else{
        _dialog_progressdlg->setValue(count % PROGRESS_MAX);
    }

}

void ProTreeWidget::SlotFinishOpenProgress()
{
    if(!_dialog_progressdlg){
        return;
    }
    _dialog_progressdlg->setValue(PROGRESS_MAX);
    delete  _dialog_progressdlg;
    _dialog_progressdlg = nullptr;
}

void ProTreeWidget::SlotCancelOPenProgress()
{

}






