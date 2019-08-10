#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/prev_mesh.h"
#include "ui/prev_mat.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("OpenGL Engine");
    project = new Ui_Project();
    project->setupTree(this, ui->treeWidget);
    setFocusPolicy(Qt::ClickFocus);
    QObject::connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(checkself(QTreeWidgetItem* ,int)));
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

void MainWindow::checkself(QTreeWidgetItem* item,int idx)
{
    string path = item->text(0).toStdString();
    size_t indx = path.rfind(".");
    string post = "";
    if(indx>0) post = path.substr(indx+1);
    while(item->parent())
    {
        item=item->parent();
        path = item->text(0).toStdString() +"/"+path;
    }
    std::cout<<path<<" "<<idx<<std::endl;
    ui->path->setAutoFillBackground(true);
    ui->path->setText(QString(path.c_str()));
    ui->textEdit->setText(QString(path.c_str()));
    if(indx>0)
    {
        if(post == "mesh")
        {
            string txt = UI_UtilMesh(path);
            ui->textEdit->setText(QString(txt.c_str()));
        }
        else if(post == "mat")
        {
            string txt = UI_UtilMat(path);
            ui->textEdit->setText(QString(txt.c_str()));
        }
        else if(post == "sum")
        {
            string txt = UI_UtilSummary(path);
            ui->textEdit->setText(QString(txt.c_str()));
        }
    }
}
