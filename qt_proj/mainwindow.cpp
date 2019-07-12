#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("OpenGL Engine");
    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(ToggleRadio1()));
    connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(ToggleRadio2()));
    connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(ToggleRadio3()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    this->ui->widget->KeyboardEvent(ev->key());
}


void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Q || ev->key() == Qt::Key_Escape)
    {
        close();
    }
}


void MainWindow::ToggleRadio1()
{
    this->ui->widget->ExterHandleClick(0);
}

void MainWindow::ToggleRadio2()
{
     this->ui->widget->ExterHandleClick(1);
}

void MainWindow::ToggleRadio3()
{
     this->ui->widget->ExterHandleClick(2);
}
