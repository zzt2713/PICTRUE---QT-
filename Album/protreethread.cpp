#include "protreethread.h"
#include <QDir>
#include <string>
#include "protreeitem.h"
#include "const.h"
ProTreeThread::ProTreeThread(const QString &src_path, const QString &dist_path,
    QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root,
    QObject *parent): QThread (parent),_src_path(src_path),_dist_path(dist_path),
    _file_count(file_count),_parent_item(parent_item),_self(self),_root(root),_bstop(false)
{

}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::run()
{
    CreateProTree(_src_path,_dist_path,_parent_item,_file_count,_self,_root);
    if(_bstop){
        auto path = dynamic_cast<ProTreeItem*> (_root) ->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    QMetaObject::invokeMethod(this, [this]() {
        emit SigUpdateItemExpanded();
    }, Qt::QueuedConnection);

    emit SigFinishProgress(_file_count);
}

void ProTreeThread::CreateProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *preItem)
{
    if(_bstop){
        return ;
    }
    bool needcopy = true;
    if(src_path == dist_path){
        needcopy = false;
    }
    QDir import_dir(src_path);
    QStringList nameFilters;
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList();

    // 分开处理文件夹和文件
    QFileInfoList dirs;
    QFileInfoList files;

    for(int i = 0;i < list.size();++i){
        if(_bstop){
            return ;
        }
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.isDir()){
            dirs.append(fileInfo);
        } else {
            const QString &suffix = fileInfo.completeSuffix();
            // 只处理图片文件
            if(suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "gif" ){
                files.append(fileInfo);
            }
        }
    }

    // 先处理所有文件夹
    for(int i = 0;i < dirs.size();++i){
        if(_bstop){
            return ;
        }
        QFileInfo fileInfo = dirs.at(i);

        ++file_count;
        emit SigUpdateProgress(file_count);
        QDir dist_dir(dist_path);
        //构造子目的路径
        QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName());
        qDebug()<< "sub_dist_path " << sub_dist_path;
        //子目的目录
        QDir sub_dist_dir(sub_dist_path);

        if(!sub_dist_dir.exists()){
            //可以创建多级目录
            bool ok = sub_dist_dir.mkpath(sub_dist_path);
            if(!ok){
                Logger::getInstance().log("sub_dist_dir mkpath failed");
                continue;
            }
        }

        auto * item = new ProTreeItem(parent_item, fileInfo.fileName(),
                                     sub_dist_path, root,TreeItemDir);
        item->setData(0,Qt::DisplayRole, fileInfo.fileName());
        item->setData(0,Qt::DecorationRole, QIcon(":/icon/dir.png"));
        item->setData(0,Qt::ToolTipRole, sub_dist_path);

        // 递归处理子目录
        CreateProTree(fileInfo.absoluteFilePath(), sub_dist_path, item, file_count, self,root,preItem);
        if (parent_item && parent_item->childCount() > 0) {
            // 延迟发送信号，确保所有子项都已添加
            QMetaObject::invokeMethod(this, [this]() {
                emit SigUpdateItemExpanded();
            }, Qt::QueuedConnection);
        }
    }

    // 再处理所有图片文件
    for(int i = 0;i < files.size();++i){
        if(_bstop){
            return ;
        }
        QFileInfo fileInfo = files.at(i);

        ++file_count;
        emit SigUpdateProgress(file_count);
        if(!needcopy){
            continue;
        }
        Logger::getInstance().log("正在导入第" + std::to_string(file_count) + "张图片...",INFO);

        QDir dist_dir(dist_path);
        QString dist_file_path = dist_dir.absoluteFilePath(fileInfo.fileName());
        if(!QFile::copy(fileInfo.absoluteFilePath(),dist_file_path)){
            continue;
        }

        auto *item = new ProTreeItem(parent_item,fileInfo.fileName(),
                                     dist_file_path,root,TreeItemPic);

        item->setData(0,Qt::DisplayRole,fileInfo.fileName());
        item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
        item->setData(0,Qt::ToolTipRole,dist_file_path);

        if(preItem){
            auto *pre_proitem = dynamic_cast<ProTreeItem*>(preItem);
            pre_proitem->SetNextItem(item);
        }
        item->SetPreItem(preItem);
        preItem = item;
    }
    Logger::getInstance().log("图片导入成功，共有" + std::to_string(file_count) + "张",INFO);
    Logger::getInstance().log("目录生成完成",INFO);
}

void ProTreeThread::SlotCancelProgress()
{
    this->_bstop = true;
}

// void ProTreeThread::CreateProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *preItem)
// {
//     if(_bstop){
//         return ;
//     }
//     bool needcopy = true;
//     if(src_path == dist_path){
//         needcopy = false;
//     }
//     QDir import_dir(src_path);
//     QStringList nameFilters;
//     import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
//     import_dir.setSorting(QDir::Name);
//     QFileInfoList list = import_dir.entryInfoList();
//     for(int i = 0;i < list.size();++i){
//         if(_bstop){
//             return ;
//         }

//         QFileInfo fileInfo = list.at(i);
//         bool bIsDir = fileInfo.isDir();
//         if(bIsDir){
//             //如果是文件夹
//             if(_bstop){
//                 return ;
//             }
//             ++file_count;
//             emit SigUpdateProgress(file_count);
//             QDir dist_dir(dist_path);
//             //构造子目的路径
//             QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName());
//             qDebug()<< "sub_dist_path " << sub_dist_path;
//             //子目的目录
//             QDir sub_dist_dir(sub_dist_path);

//             if(!sub_dist_dir.exists()){
//                 //可以创建多级目录
//                 bool ok = sub_dist_dir.mkpath(sub_dist_path);
//                 if(!ok){
//                     Logger::getInstance().log("sub_dist_dir mkpath failed");
//                     continue;
//                 }
//             }

//             auto * item = new ProTreeItem(parent_item, fileInfo.fileName(),
//                                          sub_dist_path, root,TreeItemDir);
//             item->setData(0,Qt::DisplayRole, fileInfo.fileName());
//             item->setData(0,Qt::DecorationRole, QIcon(":/icon/dir.png"));
//             item->setData(0,Qt::ToolTipRole, sub_dist_path);
//             CreateProTree(fileInfo.absoluteFilePath(), sub_dist_path, item, file_count, self,root,preItem);
//         }else{
//             if(_bstop){
//                 return ;
//             }
//             const QString &suffix = fileInfo.completeSuffix();
//             //判断文件后缀是否为图片格式
//             if(suffix != "png" && suffix != "jpg" && suffix != "jpeg" && suffix != "gif"){
//                 continue;
//             }
//             ++file_count;
//             emit SigUpdateProgress(file_count);
//             if(!needcopy){
//                 continue;
//             }
//             Logger::getInstance().log("正在导入第" + std::to_string(file_count) + "张图片...",INFO);

//             QDir dist_dir(dist_path);
//             QString dist_file_path = dist_dir.absoluteFilePath(fileInfo.fileName());
//             if(!QFile::copy(fileInfo.absoluteFilePath(),dist_file_path)){
//                 continue;
//             }

//             auto *item = new ProTreeItem(parent_item,fileInfo.fileName(),
//                                 dist_file_path,root,TreeItemPic);

//             item->setData(0,Qt::DisplayRole,fileInfo.fileName());
//             item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
//             item->setData(0,Qt::ToolTipRole,dist_file_path);

//             if(preItem){
//                 auto *pre_proitem = dynamic_cast<ProTreeItem*>(preItem);
//                 pre_proitem->SetNextItem(item);
//             }
//             item->SetPreItem(preItem);
//             preItem = item;
//         }
//     }
//     Logger::getInstance().log("图片导入成功，共有" + std::to_string(file_count) + "张",INFO);
//     Logger::getInstance().log("目录生成完成",INFO);
// }
