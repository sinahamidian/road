#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(-180,180);
    ui->horizontalSlider_3->setRange(-180,180);
    ui->horizontalSlider_2->setRange(-180,180);
}

void MainWindow::createRoad(){
    glwidget = new MyGLWidget(numberOfLines, roadWidth,x,z,t,p,e);
    glwidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{

    ui->label->setText(QString::number(value));
    glwidget->camalpha = -(3.14159265/180)*value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();

}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_3->setText(QString::number(value));
    glwidget->camteta = (3.14159265/180)*value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_2->setText(QString::number(value));
    glwidget->cambeta = -(3.14159265/180)*value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_pushButton_clicked()
{
    cout<<"update";
    glwidget->camx=ui->xm->text().toFloat();
    glwidget->camy=ui->ym->text().toFloat();
    glwidget->camz=ui->zm->text().toFloat();
    glwidget->F = ui->F->text().toFloat();
    glwidget->sx = (float)1/ui->Sx->text().toFloat();
    glwidget->sy = (float)1/ui->Sy->text().toFloat();
    glwidget->cx = ui->cx->text().toFloat();
    glwidget->cy = ui->cy->text().toFloat();

    glwidget->k1 = ui->k1->text().toFloat();
    glwidget->k2 = ui->k2->text().toFloat();
    glwidget->k3 = ui->k3->text().toFloat();

    glwidget->p1 = ui->p1->text().toFloat();
    glwidget->p2 = ui->p2->text().toFloat();


    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_pushButton_2_clicked()
{
    MainWindow *mw;
    GLfloat rw;
    int nl;
    roadWidth = ui->lineEdit->text().toFloat();
    numberOfLines = ui->lineEdit_2->text().toInt();
    x = ui->lineEdit_3->text().toFloat();
    z = ui->lineEdit_4->text().toFloat();
    t = ui->lineEdit_5->text().toFloat();
    p = ui->lineEdit_6->text().toFloat();
    e = ui->lineEdit_7->text().toFloat();

    createRoad();

}
