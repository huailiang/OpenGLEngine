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
    ext/glad.c \
    common.cpp \
    std/camera.cpp \
    std/light.cpp \
    mainwindow.cpp \
    enginewidget.cpp \
    std/skybox.cpp \
    std/texture.cpp \
    std/shader.cpp

HEADERS += \
    common.h \
    profile.h \
    std/shader.h \
    ext/stb_image.h \
    std/texture.h \
    std/camera.h \
    std/light.h \
    mainwindow.h \
    enginewidget.h \
    std/skybox.h

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
