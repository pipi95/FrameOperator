#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T15:41:17
#
#-------------------------------------------------

QT       += network serialport

QT       -= gui

TARGET = FrameOperator
TEMPLATE = lib

DEFINES += FRAMEOPERATOR_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    equipframe.cpp \
    framesender.cpp \
    deviceutility.cpp \
    mmcFrameFinder.cpp \
    mmcFrameStructure.cpp \
    framereceivermmc.cpp \
    dgpsframetrait.cpp \
    seatassignframetrait.cpp

HEADERS += \
    equipframe.h \
    framesender.h \
    deviceutility.h \
    frameoperator_global.h \
    mmcFrameFinder.h \
    mmcFrameStructure.h \
    mmcTemplateClass.h \
    framereceivermmc.h \
    dgpsframetrait.h \
    seatassignframetrait.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
