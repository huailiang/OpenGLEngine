#-------------------------------------------------
#
# Project created by QtCreator 2019-05-26T22:35:19
#
#-------------------------------------------------

QT       += core gui    \
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_engine
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    ../engine/ext/glad.c \
    ../engine/ext/xml/tinyxml.cpp \
    ../engine/ext/xml/tinystr.cpp \
    ../engine/ext/xml/tinyxmlerror.cpp \
    ../engine/ext/xml/tinyxmlparser.cpp \
    ../engine/engine.cpp \
    ../engine/std/camera.cpp \
    ../engine/std/light.cpp \
    ../engine/std/util.cpp \
    ../engine/std/texmgr.cpp \
    ../engine/std/skybox.cpp \
    ../engine/std/texture.cpp \
    ../engine/std/shader.cpp \
    ../engine/std/avatar.cpp \
    ../engine/std/material.cpp \
    ../engine/std/asset.cpp \
    ../engine/std/mesh.cpp \
    ../engine/gui/eventmgr.cpp \
    ../engine/gui/button.cpp \
    ../engine/gui/label.cpp \
    ../engine/gui/ttfont.cpp \
    ../engine/gui/uibase.cpp \
    ../engine/gui/uimgr.cpp \
    ../engine/gui/uievent.cpp \
    mainwindow.cpp \
    enginewidget.cpp \

HEADERS += \
    ../engine/profile.h \
    ../engine/std/shader.h \
    ../engine/ext/stb_image.h \
    ../engine/ext/xml/tinyxml.h \
    ../engine/ext/xml/tinystr.h \
    ../engine/ext/tiny_obj_loader.h \
    ../engine/std/texture.h \
    ../engine/std/camera.h \
    ../engine/std/light.h \
    ../engine/std/util.h \
    ../engine/std/texmgr.h \
    ../engine/std/skybox.h \
    ../engine/std/asset.h \
    ../engine/engine.h \
    ../engine/std/avatar.h \
    ../engine/std/mesh.h \
    ../engine/std/material.h \
    ../engine/gui/eventmgr.h \
    ../engine/gui/button.h \
    ../engine/gui/label.h \
    ../engine/gui/ttfont.h \
    ../engine/gui/uibase.h \
    ../engine/gui/uimgr.h \
    ../engine/gui/uievent.h \
    mainwindow.h \
    enginewidget.h \

FORMS += \
    mainwindow.ui \
    enginewidget.ui

DEFINES += _QT_EDIT_

INCLUDEPATH += /usr/local/include/
LIBS+="$$PWD/../lib/libfreetype.6.dylib"

debug: DBG = -dgb
GL = -gl4
release_gl330: GL = -gl3

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
