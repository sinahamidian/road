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

    connect(ui->openfile, SIGNAL(triggered()), this, SLOT(on_openfile_clicked()));
    connect(ui->savefile, SIGNAL(triggered()), this, SLOT(on_savefile_clicked()));

    x = 0;
    z = 10;
    roadWidth = 10;
    numberOfLines = 2;
    t = 5;
    p = 3;
    e = 2;

    ui->lineEdit->setText(QString::number(roadWidth));
    ui->lineEdit_2->setText(QString::number(numberOfLines));
    ui->lineEdit_3->setText(QString::number(x));
    ui->lineEdit_4->setText(QString::number(z));
    ui->lineEdit_5->setText(QString::number(t));
    ui->lineEdit_6->setText(QString::number(p));
    ui->lineEdit_7->setText(QString::number(e));


}

void MainWindow::createRoad(){
    glwidget = new MyGLWidget(numberOfLines, roadWidth,x,z,t,p,e);
    glwidget->show();

    glwidget->rotX = 0;
    glwidget->rotY = 0;
    glwidget->rotZ = 0;

    ui->x->setText(QString::number(glwidget->rotX));
    ui->y->setText(QString::number(glwidget->rotY));
    ui->z->setText(QString::number(glwidget->rotZ));

    ui->xm->setText(QString::number(glwidget->camx));
    ui->ym->setText(QString::number(glwidget->camy));
    ui->zm->setText(QString::number(glwidget->camz));

    ui->F->setText(QString::number(glwidget->F));
    ui->Sx->setText(QString::number(glwidget->sx));
    ui->Sy->setText(QString::number(glwidget->sy));
    ui->cx->setText(QString::number(glwidget->cx));
    ui->cy->setText(QString::number(glwidget->cy));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{

    ui->x->setText(QString::number(value));
    glwidget->camalpha = -(3.14159265/180)*value;
    glwidget->rotX = value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();

}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->z->setText(QString::number(value));
    glwidget->camteta = (3.14159265/180)*value;
    glwidget->rotZ = value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->y->setText(QString::number(value));
    glwidget->cambeta = (3.14159265/180)*value;
    glwidget->rotY = value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_pushButton_clicked()
{
    glwidget->w = ui->imxx->text().toInt();
    glwidget->h = ui->imyy->text().toInt();
    glwidget->resizeaa(glwidget->h,glwidget->w);

    QSize a(glwidget->w,glwidget->h);

    glwidget->resize(a);

    glwidget->camx=ui->xm->text().toFloat();
    glwidget->camy=ui->ym->text().toFloat();
    glwidget->camz=ui->zm->text().toFloat();
    glwidget->F = ui->F->text().toFloat();
    glwidget->sx = (float)1/ui->Sx->text().toFloat();
    glwidget->sy = (float)1/ui->Sy->text().toFloat();
    glwidget->cx = ui->cx->text().toFloat();
    glwidget->cy = ui->cy->text().toFloat();
    glwidget->rotX = ui->x->text().toFloat();
    glwidget->rotY = ui->y->text().toFloat();
    glwidget->rotZ = ui->z->text().toFloat();

    glwidget->recp = ui->recp->text().toFloat();
    glwidget->rect = ui->rect->text().toFloat();
    glwidget->recx = ui->recx->text().toFloat();
    glwidget->recz = ui->recz->text().toFloat();

    glwidget->makeTheRec();

    ui->horizontalSlider->setValue(glwidget->rotX);
    ui->horizontalSlider_3->setValue(glwidget->rotY);
    ui->horizontalSlider_2->setValue(glwidget->rotZ);

    glwidget->camalpha = -((float)3.14159265/180)*glwidget->rotX;
    glwidget->cambeta = ((float)3.14159265/180)*glwidget->rotY;
    glwidget->camteta = ((float)3.14159265/180)*glwidget->rotZ;



    x = ui->lineEdit_3->text().toFloat();
    z = ui->lineEdit_4->text().toFloat();
    t = ui->lineEdit_5->text().toFloat();
    p = ui->lineEdit_6->text().toFloat();
    e = ui->lineEdit_7->text().toFloat();

    glwidget->t = t;
    glwidget->p = p;
    glwidget->e = e;
    glwidget->xx = x;
    glwidget->zz = z;

    glwidget->makeTheCar();
    glwidget->makeThePLK();
    glwidget->makeConDOTS();

    ui->lineEdit_3->setText(QString::number(x));
    ui->lineEdit_4->setText(QString::number(z));
    ui->lineEdit_5->setText(QString::number(t));
    ui->lineEdit_6->setText(QString::number(p));
    ui->lineEdit_7->setText(QString::number(e));

    ui->recp->setText(QString::number(glwidget->recp));
    ui->rect->setText(QString::number(glwidget->rect));
    ui->recx->setText(QString::number(glwidget->recx));
    ui->recz->setText(QString::number(glwidget->recz));

    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_pushButton_2_clicked()
{
    roadWidth = ui->lineEdit->text().toFloat();
    numberOfLines = ui->lineEdit_2->text().toInt();


    ui->lineEdit->setText(QString::number(roadWidth));
    ui->lineEdit_2->setText(QString::number(numberOfLines));

    createRoad();

    ui->imxx->setText(QString::number(glwidget->w));
    ui->imyy->setText(QString::number(glwidget->h));

    on_pushButton_clicked();
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if( (event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
            on_pushButton_clicked();
}

void MainWindow::on_openfile_clicked(){
    QString fileaddr;

    fileaddr = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("Text Files (*.txt)"));

    QFile file(fileaddr);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

       QTextStream in(&file);
       while (!in.atEnd()) {
           QString word;
           in>>word;
           if(word == "NumberOfLines"){
               in>>numberOfLines;
               ui->lineEdit_2->setText(QString::number(numberOfLines));
               on_pushButton_2_clicked();
           }
           else if(word == "RoadWidth"){
               in>>roadWidth;
               ui->lineEdit->setText(QString::number(roadWidth));
           }
           else if(word == "carX"){
               QString temp;
               in>>temp;
               ui->lineEdit_3->setText(temp);
           }
           else if(word == "carZ"){
               QString temp;
               in>>temp;
               ui->lineEdit_4->setText(temp);
           }
           else if(word == "carT"){
               QString temp;
               in>>temp;
               ui->lineEdit_5->setText(temp);
           }
           else if(word == "carP"){
               QString temp;
               in>>temp;
               ui->lineEdit_6->setText(temp);
           }
           else if(word == "carE"){
               QString temp;
               in>>temp;
               ui->lineEdit_7->setText(temp);
           }
           else if(word == "RotX"){
               QString temp;
               in>>temp;
               ui->x->setText(temp);
           }
           else if(word == "RotY"){
               QString temp;
               in>>temp;
               ui->y->setText(temp);
           }
           else if(word == "RotZ"){
               QString temp;
               in>>temp;
               ui->z->setText(temp);
           }
           else if(word == "RoadX"){
               QString temp;
               in>>temp;
               ui->xm->setText(temp);
           }
           else if(word == "RoadY"){
               QString temp;
               in>>temp;
               ui->ym->setText(temp);
           }
           else if(word == "RoadZ"){
               QString temp;
               in>>temp;
               ui->zm->setText(temp);
           }
           else if(word == "F"){
               QString temp;
               in>>temp;
               ui->F->setText(temp);
           }
           else if(word == "Sx"){
               QString temp;
               in>>temp;
               ui->Sx->setText(temp);
           }
           else if(word == "Sy"){
               QString temp;
               in>>temp;
               ui->Sy->setText(temp);
           }
           else if(word == "Cx"){
               QString temp;
               in>>temp;
               ui->cx->setText(temp);
           }
           else if(word == "Cy"){
               QString temp;
               in>>temp;
               ui->cy->setText(temp);
           }
           else if(word == "imW"){
               QString temp;
               in>>temp;
               ui->imxx->setText(temp);
           }
           else if(word == "imH"){
               QString temp;
               in>>temp;
               ui->imyy->setText(temp);
           }
           else if(word == "RecT"){
               QString temp;
               in>>temp;
               ui->rect->setText(temp);
           }
           else if(word == "RecP"){
               QString temp;
               in>>temp;
               ui->recp->setText(temp);
           }
           else if(word == "RecX"){
               QString temp;
               in>>temp;
               ui->recx->setText(temp);
           }
           else if(word == "RecZ"){
               QString temp;
               in>>temp;
               ui->recz->setText(temp);
           }
       }
       on_pushButton_clicked();
       on_pushButton_clicked();
}
void MainWindow::on_savefile_clicked(){
    QString fileaddr;

    fileaddr = QFileDialog::getSaveFileName(this,tr("Save"),"",tr("Text Files (*.txt)"));
    QFile file(fileaddr);
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           return;

    QTextStream out(&file);
    out<<"RoadWidth \t"<<glwidget->roadWidth<<endl
      <<"NumberOfLines \t"<<glwidget->numberOfLines<<endl
     <<"carX \t"<<glwidget->xx<<endl
    <<"carZ \t"<<glwidget->zz<<endl
    <<"carT \t"<<glwidget->t<<endl
      <<"carP \t"<<glwidget->p<<endl
        <<"carE \t"<<glwidget->e<<endl
       <<"RotX \t"<<glwidget->rotX<<endl
      <<"RotY \t"<<glwidget->rotY<<endl
     <<"RotZ \t"<<glwidget->rotZ<<endl
    <<"RoadX \t"<<ui->xm->text()<<endl
    <<"RoadY \t"<<ui->ym->text()<<endl
    <<"RoadZ \t"<<ui->zm->text()<<endl
    <<"F \t"<<ui->F->text()<<endl
    <<"Sx \t"<<ui->Sx->text()<<endl
    <<"Sy \t"<<ui->Sy->text()<<endl
    <<"Cx \t"<<ui->cx->text()<<endl
    <<"Cy \t"<<ui->cy->text()<<endl
    <<"imW \t"<<ui->imxx->text()<<endl
    <<"imH \t"<<ui->imyy->text()<<endl
    <<"RecT \t"<<ui->rect->text()<<endl
    <<"RecP \t"<<ui->recp->text()<<endl
    <<"RecX \t"<<ui->recx->text()<<endl
    <<"RecZ \t"<<ui->recz->text()<<endl;
}
