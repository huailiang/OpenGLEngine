#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include "../engine/std/shader.hpp"
#include "../engine/std/camera.hpp"
#include "../engine/std/light.hpp"

using namespace engine;

namespace Ui {
class GLWidget;
}


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{

    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent=0);
    ~GLWidget();
    void HandleClick(int evtid);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyReleaseEvent(QKeyEvent *ev);
    virtual void timerEvent(QTimerEvent *);

private:
    void InitShader();

private:
    GLuint cubeVbo, cubeVao;
    LightShader* shader;
    Camera* camera;
    Light* light;
    unsigned int texture1, texture2;
    int m_timerId;
};

#endif //
