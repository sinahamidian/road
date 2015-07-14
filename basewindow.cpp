#include "basewindow.h"
#include "ui_basewindow.h"


BaseWindow::BaseWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseWindow)
{
    ui->setupUi(this);
}

BaseWindow::~BaseWindow()
{
    delete ui;
}

void BaseWindow::on_pushButton_clicked()
{
    MainWindow *mw;
    GLfloat rw;
    int nl;
    GLfloat x,z,t,p,e;
    rw = ui->lineEdit->text().toFloat();
    nl = ui->lineEdit_2->text().toInt();
    x = ui->lineEdit_3->text().toFloat();
    z = ui->lineEdit_4->text().toFloat();
    t = ui->lineEdit_5->text().toFloat();
    p = ui->lineEdit_6->text().toFloat();
    e = ui->lineEdit_7->text().toFloat();

    mw = new MainWindow;
    mw->numberOfLines = nl;
    mw->roadWidth = rw;
    mw->x = x;
    mw->z = z;
    mw->t = t;
    mw->p = p;
    mw->e = e;

    mw->createRoad();
    mw->show();
    this->hide();
}
