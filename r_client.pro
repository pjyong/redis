#-------------------------------------------------
#
# Project created by QtCreator 2019-08-26T14:27:54
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets




TARGET = r_client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# DEFINES  += QT_NO_DEBUG_OUTPUT
#DEFINES += NDEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    serverconfigdialog.cpp \
    setting.cpp \
    logger.cpp \
    keyrenamedialog.cpp \
    redis.cpp \
    keyactionwidget.cpp \
    keyeditstringdialog.cpp \
    keyedithashdialog.cpp \
    keyeditdialog.cpp \
    keyeditlistdialog.cpp \
    keyeditsetdialog.cpp \
    keyeditzsetdialog.cpp

HEADERS += \
        mainwindow.h \
    serverconfigdialog.h \
    setting.h \
    logger.h \
    keyrenamedialog.h \
    redis.h \
    keyactionwidget.h \
    keyeditstringdialog.h \
    keyedithashdialog.h \
    keyeditdialog.h \
    keyeditlistdialog.h \
    keyeditsetdialog.h \
    keyeditzsetdialog.h

FORMS += \
        mainwindow.ui \
    serverconfigdialog.ui \
    keyrenamedialog.ui \
    keyeditstringdialog.ui \
    keyedithashdialog.ui \
    keyeditlistdialog.ui \
    keyeditsetdialog.ui \
    keyeditzsetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    qredisclient/lua/namespace_scan.lua

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/qredisclient/myfile/release/ -lqredisclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/qredisclient/myfile/debug/ -lqredisclient
else:unix: LIBS += -L$$PWD/thirdparty/qredisclient/myfile/ -lqredisclient

INCLUDEPATH += $$PWD/thirdparty/qredisclient/src \
    $$PWD/thirdparty/qredisclient/3rdparty/asyncfuture \
    $$PWD/thirdparty/qredisclient/3rdparty/hiredis
DEPENDPATH += $$PWD/thirdparty/qredisclient/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/qredisclient/myfile/release/libqredisclient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/qredisclient/myfile/debug/libqredisclient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/qredisclient/myfile/release/qredisclient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/qredisclient/myfile/debug/qredisclient.lib
else:unix: PRE_TARGETDEPS += $$PWD/thirdparty/qredisclient/myfile/libqredisclient.a

RESOURCES += \
    static.qrc
