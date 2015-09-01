#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QApplication>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    About.setWindowTitle("About");
    About.setText(" Virtual Cam  version 1.0 \n Developed by Sina Hamidian Shoormasti   sina.hamidian@gmail.com \n Basir Tech Co    info@basirtech.com \n August 2015 ");

    qApp->installEventFilter(this);

    mutex = 0;
    ui->setupUi(this);

    started = 0;

    ui->x->setRange(-180,180);
    ui->y->setRange(-180,180);
    ui->z->setRange(-180,180);
    ui->xm->setRange(-1000,1000);
    ui->ym->setRange(-1000,1000);
    ui->zm->setRange(-1000,1000);
    ui->cx->setRange(-1000,1000);
    ui->cy->setRange(-1000,1000);
    ui->recx->setRange(-1000,1000);
    ui->recz->setRange(-1000,1000);
    ui->F->setRange(0,1000);

    ui->Sx->setSingleStep(0.1);
    ui->Sy->setSingleStep(0.1);

    ui->verticalSlider->setRange(-180,180);
    ui->verticalSlider_3->setRange(-180,180);
    ui->verticalSlider_2->setRange(-180,180);

    connect(ui->openfile, SIGNAL(triggered()), this, SLOT(on_openfile_clicked()));
    connect(ui->savefile, SIGNAL(triggered()), this, SLOT(on_savefile_clicked()));
    connect(ui->exit, SIGNAL(triggered()), this, SLOT(exitAll()));
    connect(ui->newAll, SIGNAL(triggered()), this, SLOT(newAll()));
    connect(ui->about, SIGNAL(triggered()), this, SLOT(about()));

    x = 0;
    z = 10;
    roadWidth = 10;
    numberOfLines = 2;
    t = 5;
    p = 3;
    e = 2;
    roadX = 0;
    roadY = 0;
    numOfCameras = 1;
    layoutSizeX = this->width() - ui->tabWidget->width() - 20;
    layoutSizeY = this->height() - 40;

    ui->lineEdit->setText(QString::number(roadWidth));
    ui->lineEdit_2->setText(QString::number(numberOfLines));
    ui->roadX->setText(QString::number(roadX));
    ui->roadY->setText(QString::number(roadY));
    ui->lineEdit_3->setText(QString::number(x));
    ui->lineEdit_4->setText(QString::number(z));
    ui->lineEdit_5->setText(QString::number(t));
    ui->lineEdit_6->setText(QString::number(p));
    ui->lineEdit_7->setText(QString::number(e));
    ui->numOfCams->setText(QString::number(numOfCameras));


}

void MainWindow::createRoad(){
    for (int i=0; i<10; i++)
    {
        if (numOfCameras<=i*i)
        {
            gridCols = i;
            gridRows = i;
            break;
        }
        else if (numOfCameras<=i*(i+1))
        {
            gridCols = i+1;
            gridRows = i;
            break;
        }
    }

    for(int i=0; i<numOfCameras; i++){
        widgets[i] = new MyGLWidget(numberOfLines, roadWidth,x,z,t,p,e,roadX,roadY);
        widgets[i]->number = i;
        widgets[i]->mw = this;
        widgets[i]->setParent(this);
        widgets[i]->show();
        //ui->sina->addWidget(widgets[i], (int)i/gridCols, (int)i%gridCols);
        widgets[i]->ww = layoutSizeX/gridCols;
        widgets[i]->hh = layoutSizeY/gridRows;
        widgets[i]->move(QPoint( 15+ ui->tabWidget->width() +(i%gridCols)*(widgets[i]->ww+1), 40+(i/gridCols)*(widgets[i]->hh+1)) );
    }
    currentWidget = 0;
    glwidget = widgets[currentWidget];

    glwidget->show();

    glwidget->rotX = 0;
    glwidget->rotY = 0;
    glwidget->rotZ = 0;


    ui->x->setValue(glwidget->rotX);
    ui->y->setValue(glwidget->rotY);
    ui->z->setValue(glwidget->rotZ);

    ui->xm->setValue(glwidget->camx);
    ui->ym->setValue(glwidget->camy);
    ui->zm->setValue(glwidget->camz);

    ui->F->setValue(glwidget->F);
    ui->Sx->setValue(glwidget->sx);
    ui->Sy->setValue(glwidget->sy);
    ui->cx->setValue(glwidget->cx);
    ui->cy->setValue(glwidget->cy);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_verticalSlider_valueChanged(int value)
{

    ui->x->setValue(value);
    glwidget->camalpha = -(3.14159265/180)*value;
    glwidget->rotX = value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();

}


void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    ui->z->setValue(value);
    glwidget->camteta = (3.14159265/180)*value;
    glwidget->rotZ = value;
    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_verticalSlider_3_valueChanged(int value)
{
    ui->y->setValue(value);
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

    //QSize a(glwidget->w,glwidget->h);
    QSize a(glwidget->currentW,glwidget->currentH);
    glwidget->resize(a);


    for(int i=0; i<numOfCameras; i++){
        widgets[i]->recp = ui->recp->value();
        widgets[i]->rect = ui->rect->value();
        widgets[i]->recx = ui->recx->value();
        widgets[i]->recz = ui->recz->value();
    }

    glwidget->camx=ui->xm->value();
    glwidget->camy=ui->ym->value();
    glwidget->camz=ui->zm->value();
    glwidget->F = ui->F->value();
    glwidget->sx = ui->Sx->value();
    glwidget->sy = ui->Sy->value();
    glwidget->cx = ui->cx->value();
    glwidget->cy = ui->cy->value();
    glwidget->rotX = ui->x->value();
    glwidget->rotY = ui->y->value();
    glwidget->rotZ = ui->z->value();

    glwidget->makeTheRec();

    ui->verticalSlider->setValue(glwidget->rotX);
    ui->verticalSlider_3->setValue(glwidget->rotY);
    ui->verticalSlider_2->setValue(glwidget->rotZ);

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

    ui->recp->setValue(glwidget->recp);
    ui->rect->setValue(glwidget->rect);
    ui->recx->setValue(glwidget->recx);
    ui->recz->setValue(glwidget->recz);

    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::on_pushButton_2_clicked()
{
    started = 1;
    roadWidth = ui->lineEdit->text().toFloat();
    numberOfLines = ui->lineEdit_2->text().toInt();
    roadX = ui->roadX->text().toFloat();
    roadY = ui->roadY->text().toFloat();

    numOfCameras = ui->numOfCams->text().toInt();
    widgets = new MyGLWidget* [numOfCameras];

    ui->lineEdit->setText(QString::number(roadWidth));
    ui->lineEdit_2->setText(QString::number(numberOfLines));

    createRoad();

    ui->imxx->setText(QString::number(glwidget->w));
    ui->imyy->setText(QString::number(glwidget->h));

    for(int i=0;i<numOfCameras;i++){
        glwidget = widgets[i];
        updateWidget();
    }
    glwidget = widgets[0];
    CREATED = 1;
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if( (event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
            on_pushButton_clicked();    
}
void MainWindow::exitAll(){
    exit(1);
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
           }
           else if(word == "RoadWidth"){
               in>>roadWidth;
               ui->lineEdit->setText(QString::number(roadWidth));
           }
           else if(word == "carX"){
               QString temp;
               in>>temp;
               x = temp.toFloat();
               ui->lineEdit_3->setText(temp);
           }
           else if(word == "carZ"){
               QString temp;
               in>>temp;
               z = temp.toFloat();
               ui->lineEdit_4->setText(temp);
           }
           else if(word == "carT"){
               QString temp;
               in>>temp;
               t = temp.toFloat();
               ui->lineEdit_5->setText(temp);
           }
           else if(word == "carP"){
               QString temp;
               in>>temp;
               p = temp.toFloat();
               ui->lineEdit_6->setText(temp);
           }
           else if(word == "carE"){
               QString temp;
               in>>temp;
               e = temp.toFloat();

               ui->lineEdit_7->setText(temp);
               on_pushButton_2_clicked();
           }
           else if(word == "RotX"){
               QString temp;
               in>>temp;
               glwidget->rotX = temp.toDouble();
               if(glwidget == widgets[0])
                    ui->x->setValue(temp.toDouble());
           }
           else if(word == "RotY"){
               QString temp;
               in>>temp;
               glwidget->rotY = temp.toDouble();
               if(glwidget == widgets[0])
                  ui->y->setValue(temp.toDouble());
           }
           else if(word == "RotZ"){
               QString temp;
               in>>temp;
               glwidget->rotZ = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->z->setValue(temp.toDouble());
           }
           else if(word == "CamX"){
               QString temp;
               in>>temp;
               glwidget->camx = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->xm->setValue(temp.toDouble());
           }
           else if(word == "CamY"){
               QString temp;
               in>>temp;
               glwidget->camy = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->ym->setValue(temp.toDouble());
           }
           else if(word == "CamZ"){
               QString temp;
               in>>temp;
               glwidget->camz = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->zm->setValue(temp.toDouble());
           }
           else if(word == "F"){
               QString temp;
               in>>temp;
               glwidget->F = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->F->setValue(temp.toDouble());
           }
           else if(word == "Sx"){
               QString temp;
               in>>temp;
               glwidget->sx = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->Sx->setValue(temp.toDouble());
           }
           else if(word == "Sy"){
               QString temp;
               in>>temp;
               glwidget->sy = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->Sy->setValue(temp.toDouble());
           }
           else if(word == "Cx"){
               QString temp;
               in>>temp;
               glwidget->cx = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->cx->setValue(temp.toDouble());
           }
           else if(word == "Cy"){
               QString temp;
               in>>temp;
               glwidget->cy = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->cy->setValue(temp.toDouble());
           }
           else if(word == "imW"){
               QString temp;
               in>>temp;
               glwidget->w = temp.toDouble();
               if(glwidget == widgets[0])
                    ui->imxx->setText(temp);
           }
           else if(word == "imH"){
               QString temp;
               in>>temp;
               glwidget->h = temp.toDouble();
               if(glwidget == widgets[0])
                   ui->imyy->setText(temp);
           }
           else if(word == "RecT"){
               QString temp;
               in>>temp;
               ui->rect->setValue(temp.toDouble());
               for(int i=0; i<numOfCameras; i++)
               {
                   widgets[i]->rect = temp.toDouble();
               }
           }
           else if(word == "RecP"){
               QString temp;
               in>>temp;
               ui->recp->setValue(temp.toDouble());
               for(int i=0; i<numOfCameras; i++)
               {
                   widgets[i]->recp = temp.toDouble();
               }
           }
           else if(word == "RecX"){
               QString temp;
               in>>temp;
               ui->recx->setValue(temp.toDouble());
               for(int i=0; i<numOfCameras; i++)
               {
                   widgets[i]->recx = temp.toDouble();
               }
           }
           else if(word == "RecZ"){
               QString temp;
               in>>temp;
               ui->recz->setValue(temp.toDouble());
               for(int i=0; i<numOfCameras; i++)
               {
                   widgets[i]->recz = temp.toDouble();
               }
           }
           else if(word == "rX"){
               in>>roadX;
                ui->roadX->setText(QString::number(roadX));
           }
           else if(word == "rY"){
               in>>roadY;
               ui->roadY->setText(QString::number(roadY));
           }
           else if (word == "numOfCams"){
               in>>numOfCameras;
               ui->numOfCams->setText(QString::number(numOfCameras));
           }
           else if (word == "//"){
               int a;
               in>>a;
               glwidget = widgets[a];
           }
       }
       for(int i=0;i<numOfCameras;i++){
           glwidget = widgets[i];
           updateWidget();
           updateWidget();
           //glwidget->selected();
       }
       glwidget = widgets[0];
        ui->tabWidget->setCurrentIndex(1);
}
void MainWindow::on_savefile_clicked(){
    QString fileaddr;

    fileaddr = QFileDialog::getSaveFileName(this,tr("Save"),"",tr("Text Files (*.txt)"));
    QFile file(fileaddr);
       if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
           return;

    QTextStream out(&file);
    out<<"numOfCams \t"<<numOfCameras<<endl
          <<"RoadWidth \t"<<glwidget->roadWidth<<endl
      <<"NumberOfLines \t"<<glwidget->numberOfLines<<endl
     <<"rX \t"<<ui->roadX->text()<<endl
     <<"rY \t"<<ui->roadY->text()<<endl
     <<"carX \t"<<glwidget->xx<<endl
    <<"carZ \t"<<glwidget->zz<<endl
    <<"carT \t"<<glwidget->t<<endl
      <<"carP \t"<<glwidget->p<<endl
        <<"carE \t"<<glwidget->e<<endl
       <<"RecT \t"<<glwidget->rect<<endl
       <<"RecP \t"<<glwidget->recp<<endl
       <<"RecX \t"<<glwidget->recx<<endl
       <<"RecZ \t"<<glwidget->recz<<endl;

    MyGLWidget* temp;
    temp = glwidget;
    for(int i=0;i<numOfCameras;i++){
        glwidget = widgets[i];
        out<<"// "<<i<<endl
         <<"RotX \t"<<glwidget->rotX<<endl
          <<"RotY \t"<<glwidget->rotY<<endl
         <<"RotZ \t"<<glwidget->rotZ<<endl
        <<"CamX \t"<<glwidget->camx<<endl
        <<"CamY \t"<<glwidget->camy<<endl
        <<"CamZ \t"<<glwidget->camz<<endl
        <<"F \t"<<glwidget->F<<endl
        <<"Sx \t"<<glwidget->sx<<endl
        <<"Sy \t"<<glwidget->sy<<endl
        <<"Cx \t"<<glwidget->cx<<endl
        <<"Cy \t"<<glwidget->cy<<endl
        <<"imW \t"<<glwidget->w<<endl
        <<"imH \t"<<glwidget->h<<endl;

    }
    glwidget = temp;
}

void MainWindow::on_z_valueChanged(double arg1)
{
    glwidget->rotZ = arg1;
    glwidget->camteta = (3.14159265/180)*glwidget->rotZ;
    ui->verticalSlider_2->setValue(glwidget->rotZ);
    updateWidget();
}

void MainWindow::on_y_valueChanged(double arg1)
{
    glwidget->rotY = arg1;
    glwidget->cambeta = (3.14159265/180)*glwidget->rotY;
    ui->verticalSlider_3->setValue(glwidget->rotY);
    updateWidget();
}

void MainWindow::on_x_valueChanged(double arg1)
{
    glwidget->rotX = arg1;
    glwidget->camalpha = -(3.14159265/180)*glwidget->rotX;
    ui->verticalSlider->setValue(glwidget->rotX);
    updateWidget();
}

void MainWindow::on_xm_valueChanged(double arg1)
{
    glwidget->camx = arg1;
    updateWidget();
}

void MainWindow::on_ym_valueChanged(double arg1)
{
    glwidget->camy = arg1;
    updateWidget();
}

void MainWindow::on_zm_valueChanged(double arg1)
{
    glwidget->camz = arg1;
    updateWidget();
}

void MainWindow::on_F_valueChanged(double arg1)
{
    glwidget->F = arg1;
    updateWidget();
}

void MainWindow::on_Sx_valueChanged(double arg1)
{
    glwidget->sx = arg1;
    updateWidget();
}

void MainWindow::on_Sy_valueChanged(double arg1)
{
    glwidget->sy = arg1;
    updateWidget();
}

void MainWindow::on_cx_valueChanged(double arg1)
{
    glwidget->cx = arg1;
    updateWidget();
}

void MainWindow::on_cy_valueChanged(double arg1)
{
    glwidget->cy = arg1;
    updateWidget();
}

void MainWindow::on_recx_valueChanged(double arg1)
{
    glwidget->recx = arg1;
    glwidget->makeTheRec();
    MyGLWidget *temp;
    temp = glwidget;
    for(int i=0; i<numOfCameras; i++){
        widgets[i]->recp = ui->recp->value();
        widgets[i]->rect = ui->rect->value();
        widgets[i]->recx = ui->recx->value();
        widgets[i]->recz = ui->recz->value();
    }
    for(int i=0;i<numOfCameras;i++){
        glwidget = widgets[i];
        updateWidget2();
        updateWidget2();
    }
    glwidget = temp;
}

void MainWindow::on_recz_valueChanged(double arg1)
{
    glwidget->recz = arg1;
    glwidget->makeTheRec();
    updateWidget();
    MyGLWidget *temp;
    temp = glwidget;
    for(int i=0; i<numOfCameras; i++){
        widgets[i]->recp = ui->recp->value();
        widgets[i]->rect = ui->rect->value();
        widgets[i]->recx = ui->recx->value();
        widgets[i]->recz = ui->recz->value();
    }
    for(int i=0;i<numOfCameras;i++){
        glwidget = widgets[i];
        updateWidget2();
        updateWidget2();
    }
    glwidget = temp;
}

void MainWindow::on_rect_valueChanged(double arg1)
{
    glwidget->rect = arg1;
    glwidget->makeTheRec();
    updateWidget();
    MyGLWidget *temp;
    temp = glwidget;
    for(int i=0; i<numOfCameras; i++){
        widgets[i]->recp = ui->recp->value();
        widgets[i]->rect = ui->rect->value();
        widgets[i]->recx = ui->recx->value();
        widgets[i]->recz = ui->recz->value();
    }
    for(int i=0;i<numOfCameras;i++){
        glwidget = widgets[i];
        updateWidget2();
        updateWidget2();
    }
    glwidget = temp;
}

void MainWindow::on_recp_valueChanged(double arg1)
{
    glwidget->recp = arg1;
    glwidget->makeTheRec();
    updateWidget();
    MyGLWidget *temp;
    temp = glwidget;
    for(int i=0; i<numOfCameras; i++){
        widgets[i]->recp = ui->recp->value();
        widgets[i]->rect = ui->rect->value();
        widgets[i]->recx = ui->recx->value();
        widgets[i]->recz = ui->recz->value();
    }
    for(int i=0;i<numOfCameras;i++){
        glwidget = widgets[i];
        updateWidget2();
        updateWidget2();
    }
    glwidget = temp;
}

void MainWindow::updateWidget(){
    glwidget->resizeaa(glwidget->h,glwidget->w);
    QSize a(glwidget->currentW,glwidget->currentH);
    glwidget->resize(a);

    glwidget->camalpha = -((float)3.14159265/180)*glwidget->rotX;
    glwidget->cambeta = ((float)3.14159265/180)*glwidget->rotY;
    glwidget->camteta = ((float)3.14159265/180)*glwidget->rotZ;

    glwidget->t = t;
    glwidget->p = p;
    glwidget->e = e;
    glwidget->xx = x;
    glwidget->zz = z;

    glwidget->makeTheRec();
    glwidget->makeTheCar();
    glwidget->makeThePLK();
    glwidget->makeConDOTS();

    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

void MainWindow::updateWidget2(){
    glwidget->t = t;
    glwidget->p = p;
    glwidget->e = e;
    glwidget->xx = x;
    glwidget->zz = z;

    glwidget->makeTheRec();

    glwidget->camTrans();
    glwidget->Intrinsics();
    //glwidget->Distortion();
    glwidget->updateGL();
}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
// if (e->type() == QEvent::KeyPress)
//  {

//  QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);

//  switch (keyEvent->key()){
//  case Qt::Key_Right :
//      on_y_valueChanged(ui->y->value()+1);
//      break;
//  case Qt::Key_Down :
//      on_x_valueChanged(ui->x->value()-1);
//      break;
//  case Qt::Key_Up:
//      on_x_valueChanged(ui->x->value()+1);
//      break;
//  case Qt::Key_Left:
//      on_y_valueChanged(ui->y->value()-1);
//      break;
//  }

//  if(keyEvent->key() == Qt::Key_Right || keyEvent->key() == Qt::Key_Left)
//    return true;
//  }
 return false;
}

void MainWindow::resizeEvent(QResizeEvent *a){
        layoutSizeX = this->width() - ui->tabWidget->width() - 20;
        layoutSizeY = this->height() - 40;

        if(CREATED)
        {
            MyGLWidget * temp = glwidget;
            for (int i=0; i<numOfCameras; i++)
            {
                widgets[i]->ww = layoutSizeX/gridCols;
                widgets[i]->hh = layoutSizeY/gridRows;
                if(widgets[i]->MinMaxFlag == 0)
                    widgets[i]->move(QPoint( 15+ ui->tabWidget->width() +(i%gridCols)*(widgets[i]->ww+1), 40+(i/gridCols)*(widgets[i]->hh+1)) );
                glwidget = widgets[i];
                updateWidget();
            }
            glwidget = temp;
        }
}

void MainWindow::newAll()
{
    if(started == 0)
        return;
    for(int i=0; i<numOfCameras; i++)
    {
        widgets[i]->hide();
        delete widgets[i];
    }
    ui->tabWidget->setCurrentIndex(0);
    ui->activeWin->setText("1");
    started =0;
}

void MainWindow::about()
{
    About.show();
}
