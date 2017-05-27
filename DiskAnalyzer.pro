#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T21:21:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiskAnalyzer
TEMPLATE = app
CONFIG += c++11
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = bin #Target file directory
OBJECTS_DIR = build #Intermediate object files directory
MOC_DIR = moc #Intermediate moc files directory
UI_DIR = ui


SOURCES += src/main.cpp \
        src/mainwindow.cpp \
    src/showdrives.cpp \
    src/diskview.cpp \
    src/filescanprocessor.cpp \
    src/taskview.cpp \
    src/filescan/filescan.cpp \
    src/filescan/dirinfo.cpp

HEADERS  += src/mainwindow.h \
    src/showdrives.h \
    src/diskview.h \
    src/filescanprocessor.h \
    src/taskview.h \
    src/filescan/filescan.h \
    src/filescan/dirinfo.h

FORMS    += forms/mainwindow.ui \
    forms/showdrives.ui \
    forms/diskview.ui \
    forms/filescanprocessor.ui \
    forms/taskview.ui
