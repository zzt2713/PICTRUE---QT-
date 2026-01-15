#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <QDebug>
#include "Logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logger::getInstance().log("Start log...");
    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly)){
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        Logger::getInstance().log("打开文件失败",ERROR);
        qDebug() << "open file error!";
        return -1;
    }

    MainWindow w;
    w.setWindowTitle("PICTURE");
    w.showMaximized();
    return a.exec();
}
