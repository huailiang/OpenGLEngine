#include "enginewidget.h"
#include "ui_enginewidget.h"

engineWidget::engineWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::engineWidget)
{
    connect(this, SIGNAL(clicked()), this, SLOT(InnerHandleClick()));
    ui->setupUi(this);
    m_timerId = startTimer(30);
    wrap = new SceneWrap();
}

engineWidget::~engineWidget()
{
    SAFE_DELETE(ui);
    delete wrap;
}

void engineWidget::ExterHandleClick(int evtid)
{
    wrap->HandleClick(evtid);
    update();
}

void engineWidget::InnerHandleClick()
{
    update();
    wrap->OnClickGL(mousePos.x(), mousePos.y());
}

void engineWidget::mousePressEvent(QMouseEvent *ev)
{
    mousePos = QPoint(ev->x(), ev->y());
}

void engineWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(mousePos == QPoint(ev->x(), ev->y()))
        emit clicked();
}

void engineWidget::KeyboardEvent(int key)
{
    wrap->KeyBoard(key);
}


void engineWidget::initializeGL()
{
    initializeOpenGLFunctions();
    wrap->initialize();
}

void engineWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    update();
}

void engineWidget::paintGL()
{
   wrap->Draw();
}

void engineWidget::timerEvent(QTimerEvent *)
{
    update();
}


