#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QString>
#include "protree.h"
#include "wizard.h"
#include <QFileDialog>
#include "protreewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *menu_file = menuBar()->addMenu("文件(&F)");

    //创建文件
    QAction *act_create_pro = new QAction(QIcon(":/icon/createpro.png"),tr("创建项目"),this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menu_file->addAction(act_create_pro);

    //打开项目
    QAction *open_act_pro = new QAction(QIcon(":/icon/openpro.png"),tr("打开项目"),this);
    open_act_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(open_act_pro);

    //设置菜单
    QMenu *set_menu = menuBar()->addMenu("设置(&S)");
    QAction *act_music = new QAction(QIcon(":/icon/music.png"),tr("背景音乐"),this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    set_menu->addAction(act_music);

    //连接信号与槽
    connect(act_create_pro,&QAction::triggered,this,&MainWindow::createPro);
    connect(open_act_pro,&QAction::triggered,this,&MainWindow::OpenPro);

    _protree = new ProTree();
    ui->prolayout->addWidget(_protree);

    auto *tree_widget = dynamic_cast<ProTree*> (_protree)->GetTreeWidget();
    auto *pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this,&MainWindow::SigOpenPro,pro_tree_widget,&ProTreeWidget::SlotOpenPro);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createPro(bool)
{
    Wizard wizard(this);
    Logger::getInstance().log("正在创建项目...",INFO);

    wizard.setWindowTitle("创建项目");
    auto *page = wizard.page(0);
    page->setTitle("项目配置");
    //连接信号槽
    connect(&wizard,&Wizard::SigProSettings,dynamic_cast<ProTree*>(_protree),&ProTree::AddProToTree);

    //显示项目
    wizard.show();
    wizard.exec();
    //断开信号
    disconnect();
    Logger::getInstance().log("项目创建成功！",INFO);
}

void MainWindow::OpenPro(bool)
{
    Logger::getInstance().log("正在打开项目...",INFO);
    QFileDialog file;
    file.setFileMode(QFileDialog::Directory);
    file.setWindowTitle("打开项目-选择导入的文件夹");
    file.setDirectory(QDir::current());
    file.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file.exec()){
        fileNames = file.selectedFiles();
    }

    if(fileNames.length() <= 0){
        return;
    }

    QString import_path = fileNames.at(0);
    emit SigOpenPro(import_path);

    Logger::getInstance().log("打开文件成功！",INFO);
}
