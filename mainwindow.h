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
    bool eventFilter(QObject *object, QEvent *e);
    bool CREATED;
    bool started;

    QMessageBox About;


    void createRoad();
    explicit MainWindow(QWidget *parent = 0);
    void updateWidget();
    void updateWidget2();

    MyGLWidget *glwidget;
    MyGLWidget **widgets;
    Ui::MainWindow *ui;

    ~MainWindow();
    GLfloat roadWidth;
    int numberOfLines;
    GLfloat x,z,t,p,e;
    GLfloat roadX, roadY;
    int numOfCameras;
    int gridCols, gridRows;
    int currentWidget;
    int layoutSizeX, layoutSizeY;
    bool mutex;
    bool isItDone;

    void keyPressEvent(QKeyEvent *key);

public slots:
    void about();
    void exitAll();

    void newAll();

    void resizeEvent(QResizeEvent *);

    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_3_valueChanged(int value);

    void on_verticalSlider_2_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_openfile_clicked();

    void on_savefile_clicked();

private slots:
    void on_z_valueChanged(double arg1);

    void on_y_valueChanged(double arg1);

    void on_x_valueChanged(double arg1);

    void on_xm_valueChanged(double arg1);

    void on_ym_valueChanged(double arg1);

    void on_zm_valueChanged(double arg1);

    void on_F_valueChanged(double arg1);

    void on_Sx_valueChanged(double arg1);

    void on_Sy_valueChanged(double arg1);

    void on_cx_valueChanged(double arg1);

    void on_cy_valueChanged(double arg1);

    void on_recx_valueChanged(double arg1);

    void on_recz_valueChanged(double arg1);

    void on_rect_valueChanged(double arg1);

    void on_recp_valueChanged(double arg1);

private:

    QFileDialog of;

};

#endif // MAINWINDOW_H
