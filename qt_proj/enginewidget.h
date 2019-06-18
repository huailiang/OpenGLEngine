#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <iostream>
#include "../engine/std/shader.h"
#include "../engine/std/camera.h"
#include "../engine/std/light.h"
#include "../engine/std/skybox.h"
#include "../engine/gui/label.h"

using namespace engine;

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
    MeshData* cube;
    Camera* camera;
    Light* light;
    unsigned int texture1, texture2;
    Skybox* skybox;
    int m_timerId;
    UILabel* label;
    Ui::engineWidget *ui;
};

#endif // ENGINEWIDGET_H
