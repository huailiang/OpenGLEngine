#include <QApplication>
#include <QDesktopWidget>
#include <QGLFormat>
#include <QtDebug>
#include <iostream>
#include "../engine/profile.h"
#include "../engine/std/shader.h"
#include "glwidget.h"
#include "mainwindow.h"

using namespace std;
using namespace engine;

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
    delete glWidget;
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
    MainWindow window;
    window.show();

    return a.exec();
}
