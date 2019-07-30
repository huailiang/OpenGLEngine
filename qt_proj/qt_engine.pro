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
    ../engine/engineio.cpp \
    ../engine/profile.cpp \
    ../engine/std/camera.cpp \
    ../engine/std/light.cpp \
    ../engine/std/util.cpp \
    ../engine/std/texmgr.cpp \
    ../engine/std/skybox.cpp \
    ../engine/std/terrain.cpp \
    ../engine/std/texture.cpp \
    ../engine/std/shader.cpp \
    ../engine/std/animation.cpp \
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
    ui/ui_project.cpp \
    ui/prev_mat.cpp \
    ui/prev_mesh.cpp \
    mainwindow.cpp \
    enginewidget.cpp \
    scene/scenewrap.cpp

HEADERS += \
    ../engine/profile.h \
    ../engine/std/shader.hpp \
    ../engine/ext/stb_image.h \
    ../engine/ext/xml/tinyxml.h \
    ../engine/ext/xml/tinystr.h \
    ../engine/ext/tiny_obj_loader.h \
    ../engine/std/texture.hpp \
    ../engine/std/camera.hpp \
    ../engine/std/light.hpp \
    ../engine/std/util.hpp \
    ../engine/std/texmgr.hpp \
    ../engine/std/skybox.hpp \
    ../engine/std/terrain.hpp \
    ../engine/std/asset.hpp \
    ../engine/engine.h \
    ../engine/engineio.h \
    ../engine/std/avatar.hpp \
    ../engine/std/mesh.hpp \
    ../engine/std/animation.hpp \
    ../engine/std/material.hpp \
    ../engine/gui/eventmgr.hpp \
    ../engine/gui/button.hpp \
    ../engine/gui/label.hpp \
    ../engine/gui/ttfont.hpp \
    ../engine/gui/uibase.hpp \
    ../engine/gui/uimgr.hpp \
    ../engine/gui/uievent.hpp \
    ui/ui_project.h \
    ui/prev_mat.h \
    ui/prev_mesh.h \
    mainwindow.h \
    enginewidget.h \
    scene/scenewrap.h \
    scene/scenemgr.h \
    scene/scene.h \
    scene/scene1.h \
    scene/scene2.h \
    scene/scene3.h \
    scene/scene4.h \
    scene/scene5.h

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
