#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <qtimer.h>
#include <iostream>
#include "scene/scenewrap.h"

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
    void ExterHandleClick(int evtid);
    void KeyboardEvent(int key);

signals:
    void clicked();

public slots:
    void InnerHandleClick();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void timerEvent(QTimerEvent *);



protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    int m_timerId;
    Ui::engineWidget *ui;
    SceneWrap* wrap;

    QPoint mousePos;
};

#endif // ENGINEWIDGET_H
