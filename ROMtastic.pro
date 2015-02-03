#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T18:36:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ROMtastic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    errordock.cpp \
    highlighter.cpp \
    filemanager.cpp

HEADERS  += mainwindow.h \
    errordock.h \
    highlighter.h \
    filemanager.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-beCreative-Desktop-Debug/release/ -lbeCreative
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-beCreative-Desktop-Debug/debug/ -lbeCreative
else:unix: LIBS += -L$$PWD/../build-beCreative-Desktop-Debug/ -lbeCreative

INCLUDEPATH += $$PWD/../beCreative
DEPENDPATH += $$PWD/../beCreative
