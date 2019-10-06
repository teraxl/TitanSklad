#-------------------------------------------------
#
# Project created by QtCreator 2019-09-23T01:22:04
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OGL-Test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    simpleobject3d.cpp \
    group3d.cpp \
    camera3d.cpp

HEADERS  += widget.h \
    simpleobject3d.h \
    iobjecttransform.h \
    group3d.h \
    camera3d.h


win32-g* {
    LIBS += -lopengl32
}

win32-msvc* {
    LIBS += opengl32.lib
}

DISTFILES +=

RESOURCES += \
    shaders.qrc
