#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myglwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void createRoad();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GLfloat roadWidth;
    int numberOfLines;
    GLfloat x,z,t,p,e;


private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    MyGLWidget *glwidget;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
