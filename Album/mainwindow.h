#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Logger.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QMenu>
#include <QLabel>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *_protree;

    // Qt6 音乐播放相关
    QMediaPlayer *_mediaPlayer;
    QAudioOutput *_audioOutput;
    QAction *_musicAction;
    QSlider *_volumeSlider;
    QLabel *_musicStatusLabel;
    bool _isMusicPlaying;
    QAction *_playPauseAction;
    QAction *_nextMusicAction;
    QAction *_prevMusicAction;
    // 多音乐管理
    QStringList _musicList;
    int _currentMusicIndex;
    QString _currentMusicName;

    void initMusicPlayer();
    void loadMusicList();
    void switchToNextMusic();
    void switchToPrevMusic();
    void playMusicAtIndex(int index);
    void updateMusicInfoLabel();

private slots:
    void createPro(bool);
    void OpenPro(bool);
    void toggleMusic(bool);
    void updateVolume(int volume);
    void musicStateChanged(QMediaPlayer::PlaybackState state);
    void onMusicErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
    void onPlayPauseClicked();
    void loadDefaultMusic();

signals:
    void SigOpenPro(const QString &path);
};
#endif // MAINWINDOW_H
