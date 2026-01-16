#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "protree.h"
#include "wizard.h"
#include "protreewidget.h"
#include <QMenu>
#include <QAction>
#include <QString>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QStyle>
#include <QLabel>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QUrl>
#include "Logger.h"
#include <QTemporaryDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _protree(nullptr)
    , _mediaPlayer(nullptr)
    , _audioOutput(nullptr)
    , _musicAction(nullptr)
    , _volumeSlider(nullptr)
    , _musicStatusLabel(nullptr)
    , _isMusicPlaying(false)
    , _playPauseAction(nullptr)
    , _nextMusicAction(nullptr)
    , _prevMusicAction(nullptr)
    , _currentMusicIndex(0)
    , _currentMusicName("")
{
    ui->setupUi(this);

    initMusicPlayer();

    // --- 文件菜单 ---
    QMenu *menu_file = menuBar()->addMenu("文件(&F)");

    // 创建项目动作
    QAction *act_create_pro = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menu_file->addAction(act_create_pro);

    // 打开项目动作
    QAction *open_act_pro = new QAction(QIcon(":/icon/openpro.png"), tr("打开项目"), this);
    open_act_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(open_act_pro);

    // --- 设置菜单 ---
    QMenu *set_menu = menuBar()->addMenu("设置(&S)");
    _musicAction = new QAction(QIcon(":/icon/music.png"), tr("背景音乐"), this);
    _musicAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    _musicAction->setCheckable(true);
    _musicAction->setChecked(false);
    set_menu->addAction(_musicAction);


    QToolBar *musicToolBar = addToolBar("音乐");
    musicToolBar->setMovable(false);

    _prevMusicAction = new QAction(QIcon(":/icon/previous.png"), "下一首", this);
    _prevMusicAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    musicToolBar->addAction(_prevMusicAction);

    // 播放/暂停按钮
    _playPauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), "播放/暂停", this);
    musicToolBar->addAction(_playPauseAction);

    // 下一首按钮
    _nextMusicAction = new QAction(QIcon(":/icon/next.png"), "下一首", this);
    _nextMusicAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    musicToolBar->addAction(_nextMusicAction);

    // 音量标签
    QLabel *volumeLabel = new QLabel("音量: ", this);
    volumeLabel->setStyleSheet("color: white;");
    musicToolBar->addWidget(volumeLabel);

    // 音量滑块
    _volumeSlider = new QSlider(Qt::Horizontal, this);
    _volumeSlider->setRange(0, 100);
    _volumeSlider->setValue(50);
    _volumeSlider->setFixedWidth(100);
    musicToolBar->addWidget(_volumeSlider);

    _musicStatusLabel = new QLabel("音乐: 停止 | 未选择音乐", this);
    _musicStatusLabel->setStyleSheet("QLabel { color: white; margin-left: 10px; }");

    musicToolBar->addWidget(_musicStatusLabel);

    loadMusicList();


    _protree = new ProTree();


    if (ui->prolayout) {
        ui->prolayout->addWidget(_protree);
    } else {
        QWidget* central = ui->centralwidget;
        if (!central->layout()) {
            QVBoxLayout* layout = new QVBoxLayout(central);
            central->setLayout(layout);
        }
        central->layout()->addWidget(_protree);
        Logger::getInstance().log("警告: UI中未找到 prolayout，已自动添加到主布局", WARNING);
    }

    connect(act_create_pro, &QAction::triggered, this, &MainWindow::createPro);
    connect(open_act_pro, &QAction::triggered, this, &MainWindow::OpenPro);
    connect(_musicAction, &QAction::triggered, this, &MainWindow::toggleMusic);
    connect(_playPauseAction, &QAction::triggered, this, &MainWindow::onPlayPauseClicked);
    connect(_nextMusicAction, &QAction::triggered, this, &MainWindow::switchToNextMusic);
    connect(_volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);
    connect(_prevMusicAction, &QAction::triggered, this, &MainWindow::switchToPrevMusic);
    // ProTree 连接
    auto *casted_tree = dynamic_cast<ProTree*>(_protree);
    if (casted_tree) {
        auto *tree_widget = casted_tree->GetTreeWidget();

        if (auto *pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget)) {
            connect(this, &MainWindow::SigOpenPro, pro_tree_widget, &ProTreeWidget::SlotOpenPro);
        } else {
            Logger::getInstance().log("错误: 无法获取 ProTreeWidget，OpenPro 信号未连接", ERROR);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (_mediaPlayer) {
        _mediaPlayer->stop();
        delete _mediaPlayer;
    }
    if (_audioOutput) {
        delete _audioOutput;
    }
}

void MainWindow::initMusicPlayer()
{
    try {
        _mediaPlayer = new QMediaPlayer(this);
        _audioOutput = new QAudioOutput(this);
        _mediaPlayer->setAudioOutput(_audioOutput);
        _audioOutput->setVolume(0.5);

        connect(_mediaPlayer, &QMediaPlayer::playbackStateChanged,
                this, &MainWindow::musicStateChanged);
        connect(_mediaPlayer, QOverload<QMediaPlayer::Error, const QString &>::of(&QMediaPlayer::errorOccurred),
                this, &MainWindow::onMusicErrorOccurred);

        connect(_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
            // 只有当状态变成 Stopped，且媒体状态是 "EndOfMedia" (自然播放结束) 时，才自动切歌。
            // 这样可以过滤掉因为 setSource() 导致的人为 Stopped。
            if (state == QMediaPlayer::StoppedState &&
                _mediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia &&
                _musicAction->isChecked()) {

                switchToNextMusic();
            }
        });

        Logger::getInstance().log("音乐播放器初始化成功", INFO);
    } catch (const std::exception &e) {
        Logger::getInstance().log("音乐播放器初始化失败: " + std::string(e.what()), ERROR);
        QMessageBox::warning(this, "警告", "音乐播放器初始化失败，背景音乐功能将不可用。");
    }
}

void MainWindow::loadMusicList()
{
    // 添加多个音乐资源
    _musicList.clear();

    // 第一个音乐（原有的）
    _musicList << ":/music/background.m4a";
    _musicList << ":/music/1.m4a";
    _musicList << ":/music/2.m4a";
    _musicList << ":/music/3.m4a";

    // 给每个音乐设置名称
    QStringList musicNames;
    musicNames << "光年之外" << "月下白鸽" << "倒数" << "孤独";

    // 清理不存在的音乐文件
    for (int i = _musicList.size() - 1; i >= 0; i--) {
        if (!QFile::exists(_musicList[i])) {
            Logger::getInstance().log("音乐文件不存在: " + _musicList[i].toStdString(), WARNING);
            _musicList.removeAt(i);
            musicNames.removeAt(i);
        }
    }

    // 如果有音乐，设置当前音乐名称
    if (!_musicList.isEmpty()) {
        _currentMusicIndex = 0;
        _currentMusicName = musicNames.isEmpty() ? QString("音乐%1").arg(_currentMusicIndex + 1) : musicNames[_currentMusicIndex];
        updateMusicInfoLabel();
    }

    Logger::getInstance().log(QString("加载了 %1 首音乐").arg(_musicList.size()).toStdString(), INFO);
}

void MainWindow::playMusicAtIndex(int index)
{
    if (index < 0 || index >= _musicList.size()) {
        return;
    }

    QString musicPath = _musicList[index];

    // 检查是否是资源文件
    if (musicPath.startsWith(":/")) {
        // 处理资源文件（复制到临时目录）
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                           + QString("/temp_music_%1").arg(index)
                           + QFileInfo(musicPath).suffix();

        // 检查临时文件是否已经存在
        if (!QFile::exists(tempPath)) {
            if (QFile::copy(musicPath, tempPath)) {
                QFile::setPermissions(tempPath, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser);
            } else {
                Logger::getInstance().log("无法复制音乐文件到临时目录: " + musicPath.toStdString(), ERROR);
                return;
            }
        }

        musicPath = tempPath;
    }

    // 设置音乐源并播放
    _mediaPlayer->setSource(QUrl::fromLocalFile(musicPath));

    QStringList musicNames = {"光年之外", "月下白鸽 何以为家", "倒数", "孤独"};
    _currentMusicName = index < musicNames.size() ? musicNames[index] : QString("音乐%1").arg(index + 1);
    updateMusicInfoLabel();

    // 如果音乐已启用，自动开始播放
    if (_musicAction->isChecked()) {
        _mediaPlayer->play();
    }
}

void MainWindow::updateMusicInfoLabel()
{
    if (!_musicList.isEmpty()) {
        // 获取当前音量值
        int vol = _volumeSlider ? _volumeSlider->value() : 50;

        // 【修改点】在格式字符串末尾增加了 " | 音量: %4%"
        _musicStatusLabel->setText(QString("音量: %4% | 状态: %1/%2: %3")
                                       .arg(_currentMusicIndex + 1)
                                       .arg(_musicList.size())
                                       .arg(_currentMusicName)
                                       .arg(vol));
    } else {
        _musicStatusLabel->setText("音乐: 无可用音乐");
    }
}

void MainWindow::createPro(bool)
{
    Wizard wizard(this);
    Logger::getInstance().log("正在创建项目...", INFO);

    wizard.setWindowTitle("创建项目");
    auto *page = wizard.page(0);
    page->setTitle("项目配置");

    // 连接信号槽
    connect(&wizard, &Wizard::SigProSettings, dynamic_cast<ProTree*>(_protree), &ProTree::AddProToTree);

    // 显示项目
    wizard.show();
    wizard.exec();

    // 断开信号
    disconnect(&wizard, nullptr, nullptr, nullptr);
    Logger::getInstance().log("项目创建成功！", INFO);
}

void MainWindow::OpenPro(bool)
{
    Logger::getInstance().log("正在打开项目...", INFO);

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle("打开项目-选择导入的文件夹");
    fileDialog.setDirectory(QDir::current());
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (fileDialog.exec()) {
        fileNames = fileDialog.selectedFiles();
    }

    if (fileNames.length() <= 0) {
        return;
    }

    QString import_path = fileNames.at(0);
    emit SigOpenPro(import_path);

    Logger::getInstance().log("打开文件成功！", INFO);
}

void MainWindow::toggleMusic(bool checked)
{
    if (checked) {
        // 启用背景音乐
        _musicAction->setText("关闭背景音乐");
        _musicAction->setIcon(QIcon(":/icon/music.png"));

        if (!_isMusicPlaying) {
            // 如果没有当前音乐，加载第一首
            if (_musicList.isEmpty()) {
                loadMusicList();
            }

            if (!_musicList.isEmpty() && _mediaPlayer->source().isEmpty()) {
                playMusicAtIndex(_currentMusicIndex);
            }

            onPlayPauseClicked();
        }

        Logger::getInstance().log("背景音乐已启用", INFO);
    } else {
        // 关闭背景音乐
        _musicAction->setText("背景音乐");
        _musicAction->setIcon(QIcon(":/icon/left.png"));

        if (_isMusicPlaying) {
            _mediaPlayer->pause();
            _isMusicPlaying = false;
        }

        Logger::getInstance().log("背景音乐已关闭", INFO);
    }
}

void MainWindow::updateVolume(int volume)
{
    if (_audioOutput) {
        float vol = volume / 100.0f;
        _audioOutput->setVolume(vol);

        updateMusicInfoLabel();
    }
}

void MainWindow::musicStateChanged(QMediaPlayer::PlaybackState state)
{
    QString stateText;
    switch (state) {
    case QMediaPlayer::PlayingState:
        stateText = "播放";
        _isMusicPlaying = true;
        if (_playPauseAction) {
            _playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }
        break;
    case QMediaPlayer::PausedState:
        stateText = "暂停";
        _isMusicPlaying = false;
        if (_playPauseAction) {
            _playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }
        break;
    case QMediaPlayer::StoppedState:
        stateText = "停止";
        _isMusicPlaying = false;
        if (_playPauseAction) {
            _playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }
        break;
    }


    int vol = _volumeSlider ? _volumeSlider->value() : 50;

    if (!_musicList.isEmpty()) {
        _musicStatusLabel->setText(QString(" 音量: %5% | 状态: %1 | %2/%3: %4")
                                       .arg(stateText)
                                       .arg(_currentMusicIndex + 1)
                                       .arg(_musicList.size())
                                       .arg(_currentMusicName)
                                       .arg(vol));
    } else {
        _musicStatusLabel->setText(QString("音乐: %1 | 无可用音乐").arg(stateText));
    }
}
void MainWindow::onMusicErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    Q_UNUSED(error)

    Logger::getInstance().log("音乐播放错误: " + errorString.toStdString(), ERROR);
    QMessageBox::warning(this, "音乐播放错误", "无法播放音乐: " + errorString);

    // 重置状态
    _isMusicPlaying = false;
    _musicStatusLabel->setText("音乐: 错误");
    if (_musicAction) {
        _musicAction->setChecked(false);
        _musicAction->setText("背景音乐");
        _musicAction->setIcon(QIcon(":/icon/music.png"));
    }
}

void MainWindow::onPlayPauseClicked()
{
    if (!_mediaPlayer || !_audioOutput) {
        return;
    }

    // 如果音乐列表为空，先加载
    if (_musicList.isEmpty()) {
        loadMusicList();
        if (_musicList.isEmpty()) {
            QMessageBox::information(this, "提示", "没有可播放的音乐文件。");
            _musicAction->setChecked(false);
            return;
        }
    }

    if (_isMusicPlaying) {
        _mediaPlayer->pause();
        _isMusicPlaying = false;

        if (_playPauseAction) {
            _playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }

        Logger::getInstance().log("音乐暂停", INFO);
    } else {
        if (_mediaPlayer->source().isEmpty()) {
            playMusicAtIndex(_currentMusicIndex);
        }

        if (!_mediaPlayer->source().isEmpty()) {
            _mediaPlayer->play();
            _isMusicPlaying = true;

            if (_playPauseAction) {
                _playPauseAction->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            }

            Logger::getInstance().log("音乐开始播放", INFO);
        } else {
            QMessageBox::information(this, "提示", "没有可播放的音乐文件，请先选择音乐文件。");
            _musicAction->setChecked(false);
        }
    }
}

void MainWindow::loadDefaultMusic()
{
    if (!_musicList.isEmpty()) {
        playMusicAtIndex(0);
    }
}

void MainWindow::switchToPrevMusic()
{
    if (_musicList.isEmpty()) {
        QMessageBox::information(this, "提示", "没有可播放的音乐文件。");
        return;
    }

    _currentMusicIndex--;
    if (_currentMusicIndex < 0) {
        _currentMusicIndex = _musicList.size() - 1;
    }

    playMusicAtIndex(_currentMusicIndex);
    _mediaPlayer->play();
    if (_musicAction && !_musicAction->isChecked()) {
        _musicAction->setChecked(true);
        _musicAction->setText("关闭背景音乐");
    }

    Logger::getInstance().log(QString("切换到上一首 (第 %1 首)").arg(_currentMusicIndex + 1).toStdString(), INFO);
}

void MainWindow::switchToNextMusic()
{
    if (_musicList.isEmpty()) {
        QMessageBox::information(this, "提示", "没有可播放的音乐文件。");
        return;
    }

    _currentMusicIndex = (_currentMusicIndex + 1) % _musicList.size();
    playMusicAtIndex(_currentMusicIndex);
    _mediaPlayer->play();

    if (_musicAction && !_musicAction->isChecked()) {
        _musicAction->setChecked(true);
        _musicAction->setText("关闭背景音乐");
    }

    Logger::getInstance().log(QString("切换到第 %1 首音乐").arg(_currentMusicIndex + 1).toStdString(), INFO);
}
