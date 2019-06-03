#include "common.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGLFormat>
#include <QtDebug>

#include <iostream>
#include "profile.h"
#include "glwidget.h"
#include "std/shader.h"
#include "mainwindow.h"


using namespace std;

#ifdef USE_OPENGL_330
    #define GL_MAJOR 3
    #define GL_MINOR 3
#else
    #define GL_MAJOR 4
    #define GL_MINOR 1
#endif


bool checkOpenGL()
{
    QGLWidget *glWidget = new QGLWidget;
    QGLContext* glContext = (QGLContext *) glWidget->context();
    glContext->makeCurrent();

    int glMajorVersion, glMinorVersion;
    glMajorVersion = glContext->format().majorVersion();
    glMinorVersion = glContext->format().minorVersion();

    qDebug() << "Checking OpenGL version...";
    qDebug() << "Widget OpenGL:" << QString("%1.%2").arg(glMajorVersion).arg(glMinorVersion);
    qDebug() << "Context valid:" << glContext->isValid() ;
    qDebug() << "OpenGL information:" ;
    qDebug() << "VENDOR:"       << (const char*)glGetString(GL_VENDOR) ;
    qDebug() << "RENDERER:"     << (const char*)glGetString(GL_RENDERER) ;
    qDebug() << "VERSION:"      << (const char*)glGetString(GL_VERSION) ;
    qDebug() << "GLSL VERSION:" << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) ;

    delete glWidget;

    qDebug() << QString("Version: %1.%2").arg(glMajorVersion).arg(glMinorVersion);

    // check openGL version
    if( glMajorVersion < GL_MAJOR || (glMajorVersion == GL_MAJOR && glMinorVersion < GL_MINOR))
    {
        qDebug() << QString("Error: This version of AwesomeBump does not support openGL versions lower than %1.%2 :(").arg(GL_MAJOR).arg(GL_MINOR) ;
           return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QGLFormat::hasOpenGL())
    {
        cerr << "This system has no OpenGL support" << endl;
        return 1;
    }

    QGLFormat glFormat(QGL::SampleBuffers);

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
     /*
     * Commenting out the next line because it causes rendering to fail.  QGLFormat::CoreProfile
     * disables all OpenGL functions that are depreciated as of OpenGL 3.0.  This fix is a workaround.
     * The full solution is to replace all depreciated OpenGL functions with their current implements.
    */
# if defined(Q_OS_MAC)
    glFormat.setProfile( QGLFormat::CoreProfile );
# endif
    glFormat.setVersion( GL_MAJOR, GL_MINOR );
#endif

    QGLFormat::setDefaultFormat(glFormat);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    if(!checkOpenGL())
    {
        std::cout<<"Sorry but it seems that your graphics card does not support openGL "<<GL_MAJOR<<GL_MINOR;
    }

    GlobalInit();

    MainWindow window;
    window.show();

    return a.exec();
}
