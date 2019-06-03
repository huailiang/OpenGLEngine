#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <iostream>
#include "../code/std/shader.h"
#include "../code/std/camera.h"
#include "../code/std/light.h"
#include "../code/std/skybox.h"

namespace Ui {
class engineWidget;
}

class engineWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit engineWidget(QWidget *parent = nullptr);
    ~engineWidget();
    void HandleClick(int evtid);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void timerEvent(QTimerEvent *);

private:
    void InitShader();

private:
    unsigned int vbo, vao;
    LightShader* shader;
    Camera* camera;
    Light* light;
    unsigned int texture1, texture2;
    Skybox* skybox;
    int m_timerId;
    Ui::engineWidget *ui;
};

#endif // ENGINEWIDGET_H
