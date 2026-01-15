#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QLineEdit>
#include <QFileDialog>
#include <QRegularExpression>
proSetPage::proSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::proSetPage)
{
    ui->setupUi(this);

    // 设置页面固定大小
    this->setFixedSize(600, 200);

    ui->lineEdit->setPlaceholderText("请输入项目名称");

    QString style = R"(
        QWizardPage {
            background-color: #F8FAFC;
        }
        QLabel {
            color: #334155;
            font-size: 13px;
            font-weight: 500;
            padding: 2px 0;
        }
        QLineEdit {
            background-color: white;
            border: 1.5px solid #CBD5E1;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
            color: #1E293B;
            selection-background-color: #0EA5E9;
        }
        QLineEdit:focus {
            border: 1.5px solid #0EA5E9;
            background-color: #F0F9FF;
        }
        QLineEdit:hover:!focus {
            border: 1.5px solid #94A3B8;
        }
        QPushButton {
            background-color: #F1F5F9;
            border: 1.5px solid #CBD5E1;
            border-radius: 6px;
            color: #475569;
            font-size: 13px;
            font-weight: 500;
            padding: 8px 16px;
        }
        QPushButton:hover {
            background-color: #E2E8F0;
            border-color: #94A3B8;
        }
        QPushButton:pressed {
            background-color: #CBD5E1;
        }
        QLabel#tips {
            color: #64748B;
            font-size: 15px;
            padding-top: 5px;
        }
    )";

    this->setStyleSheet(style);
    ui->tips->setObjectName("tips");

    registerField("proPath", ui->lineEdit_2);
    registerField("proName*", ui->lineEdit);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &proSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &proSetPage::completeChanged);

    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath);
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size());
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);
}

proSetPage::~proSetPage()
{
    delete ui;
}

void proSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

bool proSetPage::isComplete() const
{
    // 检查输入是否为空
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()){
        ui->tips->setStyleSheet("color:#64748B;");
        ui->tips->setText("");
        return false;
    }

    // 检查路径是否存在
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists()){
        ui->tips->setStyleSheet("color:red;");
        ui->tips->setText("指定的路径不存在！");
        return false;
    }

    // 检查项目文件夹是否已存在
    QString projectFullPath = ui->lineEdit_2->text() + QDir::separator() + ui->lineEdit->text();
    QDir projectDir(projectFullPath);
    if(projectDir.exists()){
        ui->tips->setStyleSheet("color:red;");
        ui->tips->setText("项目已存在！请使用其他名称");
        return false;
    }

    // 检查项目名称是否有效
    QString projectName = ui->lineEdit->text();
    if(projectName.contains(QRegularExpression("[\\\\/:*?\"<>|]"))){
        ui->tips->setStyleSheet("color:red;");
        ui->tips->setText("项目名称包含非法字符");
        return false;
    }

    // 所有检查通过
    ui->tips->setText("");
    return true;
}

void proSetPage::on_pushButton_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    dialog.setDirectory(path);
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
    }

    if(fileNames.length() <= 0){
        return;
    }

    QString import_path = fileNames.at(0);
    qDebug() << "import_path is " << import_path;
    ui->lineEdit_2->setText(import_path);
}

