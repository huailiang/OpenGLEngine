#include "enginewidget.h"
#include "ui_enginewidget.h"

engineWidget::engineWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::engineWidget)
{
    ui->setupUi(this);
    m_timerId = startTimer(30);
    scene = new Scene();
}

engineWidget::~engineWidget()
{
    SAFE_DELETE(ui);
    delete scene;
}

void engineWidget::HandleClick(int evtid)
{
    scene->HandleClick(evtid);
    update();
}

void engineWidget::initializeGL()
{
    initializeOpenGLFunctions();
    scene->initialize();
}

void engineWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    update();
}

void engineWidget::paintGL()
{
   scene->Draw();
}


void engineWidget::timerEvent(QTimerEvent *)
{
    update();
}


