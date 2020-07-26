QT       += core gui widgets

TARGET = particle-trajectory
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        paintwidget.cpp \
        infowindow.cpp

HEADERS += \
        mainwindow.h \
        paintwidget.h \
        infowindow.h

FORMS += \
        mainwindow.ui \
        infowindow.ui

RESOURCES += \
        rsc.qrc
