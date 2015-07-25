#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class BaseWindow;
}

class BaseWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit BaseWindow(QWidget *parent = 0);
    ~BaseWindow();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::BaseWindow *ui;
};

#endif // BASEWINDOW_H
