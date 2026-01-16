QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += sql multimedia  # 添加 multimedia 到 QT 中

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logger.cpp \
    ProjectHistoryManager.cpp \
    confirmpage.cpp \
    main.cpp \
    mainwindow.cpp \
    opentreethread.cpp \
    prosetpage.cpp \
    protree.cpp \
    protreeitem.cpp \
    protreethread.cpp \
    protreewidget.cpp \
    removepro.cpp \
    wizard.cpp

HEADERS += \
    Logger.h \
    ProjectHistoryManager.h \
    confirmpage.h \
    const.h \
    mainwindow.h \
    opentreethread.h \
    prosetpage.h \
    protree.h \
    protreeitem.h \
    protreethread.h \
    protreewidget.h \
    removepro.h \
    wizard.h

FORMS += \
    confirmpage.ui \
    mainwindow.ui \
    prosetpage.ui \
    protree.ui \
    removepro.ui \
    wizard.ui

RC_ICONS = icon/head.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc
