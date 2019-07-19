#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui/ui_project.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);


public slots:
    void checkself(QTreeWidgetItem* ,int);


private:
    Ui::MainWindow *ui;
    Ui_Project *project;
};



#endif // MAINWINDOW_H
