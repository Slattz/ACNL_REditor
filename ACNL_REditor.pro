#-------------------------------------------------
#
# Project created by QtCreator 2018-06-21T02:06:36
#
#-------------------------------------------------

QT       += core gui

VERSION = 1.1.0.0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ACNL_REditor
RC_ICONS = icon.ico
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

@
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
@

CONFIG += gnu++17
QMAKE_CXXFLAGS += -fshort-wchar

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3

# Use -Os to optimise but keep size in mind too
QMAKE_CXXFLAGS_RELEASE *= -Os

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        Sources/main.cpp \
        Sources/mainwindow.cpp \
        Sources/file.cpp \
        Sources/game.cpp \
        Sources/shophourseditor.cpp \
        Sources/patch.cpp \
        Sources/aboutwindow.cpp \
        Sources/buttonremap.cpp \
        Sources/itempricechanger.cpp \
        Sources/textboxdelegate.cpp

HEADERS += \
        Headers/mainwindow.h \
        Headers/file.h \
        Headers/game.h \
        Headers/shophourseditor.h \
        Headers/patch.h \
        Headers/aboutwindow.h \
        Headers/buttonremap.h \
        Headers/itempricechanger.h \
        Headers/textboxdelegate.h

FORMS += \
        Forms/itempricechanger.ui \
        Forms/mainwindow.ui \
        Forms/shophourseditor.ui \
        Forms/aboutwindow.ui \
        Forms/buttonremap.ui
