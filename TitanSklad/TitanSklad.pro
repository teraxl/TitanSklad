#-------------------------------------------------
#
# Project created by QtCreator 2019-09-23T01:22:04
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = TitanSklad
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


win32 {
    LIBS += -lopengl32
    LIBS += -lglut
    LIBS += -lgdi32
    LIBS += -lglfw3
    LIBS += -lfreeglut
    LIBS += -lglut32

    DESTDIR = .\release
    QMAKE_POST_LINK += windeployqt --release $$OUT_PWD/$$DESTDIR
}


DISTFILES +=

RESOURCES += \
    shaders.qrc


