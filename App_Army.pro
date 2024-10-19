#-------------------------------------------------
#
# Project created by QtCreator 2024-04-03T20:09:13
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += widgets
QT       += core gui charts



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App_Army
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sqlstruct.cpp \
    login.cpp \
    register.cpp \
    ui_user.cpp

HEADERS  += mainwindow.h \
    sqlstruct.h \
    login.h \
    register.h \
    ui_user.h

FORMS    += mainwindow.ui \
    login.ui \
    register.ui \
    ui_user.ui

RESOURCES += \
    imgs.qrc

DISTFILES +=
