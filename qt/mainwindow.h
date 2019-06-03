#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void keyReleaseEvent(QKeyEvent *ev);

public slots:
    void ToggleRadio1();
    void ToggleRadio2();
    void ToggleRadio3();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
