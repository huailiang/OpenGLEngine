#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <iostream>
#include "scene.h"

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
    int m_timerId;
    Ui::engineWidget *ui;
    Scene* scene;
};

#endif // ENGINEWIDGET_H
