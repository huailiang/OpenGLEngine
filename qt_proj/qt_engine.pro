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
    ../engine/common.cpp \
    ../engine/std/camera.cpp \
    ../engine/std/light.cpp \
    mainwindow.cpp \
    enginewidget.cpp \
    ../engine/std/skybox.cpp \
    ../engine/std/texture.cpp \
    ../engine/std/shader.cpp

HEADERS += \
    ../engine/profile.h \
    ../engine/std/shader.h \
    ../engine/ext/stb_image.h \
    ../engine/std/texture.h \
    ../engine/std/camera.h \
    ../engine/std/light.h \
    mainwindow.h \
    enginewidget.h \
    ../engine/std/skybox.h \
    ../engine/common.h

FORMS += \
    mainwindow.ui \
    enginewidget.ui

DEFINES += _QT_EDIT_

INCLUDEPATH += /usr/local/include/
#LIBS+=-lopengl

debug: DBG = -dgb
GL = -gl4
release_gl330: GL = -gl3

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
