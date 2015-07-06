#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL/QtOpenGL>


struct DOT{
    GLfloat x,y,z;
};

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
     explicit MyGLWidget(QWidget *parent = 0);
    MyGLWidget(int nl, GLfloat rw, GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4 , GLfloat x5);
    void Distortion();
     void Intrinsics();
     void clear();
     void camTrans();
     void rotateAroundAnAxis(GLfloat angle, QVector3D* in, QVector3D* Axis);


     QVector3D xAxis, yAxis, zAxis;
     DOT* dots;
     DOT* dots0;
     DOT* imdots;
     int dotsSize, numberOfLines;
     GLfloat w, h, roadWidth;
     GLfloat xx, zz,  t,  p , e;
     GLfloat x,y,z ,camx,camy,camz,camteta,cambeta,camalpha;
     GLfloat F,sx,sy,cx,cy;
     GLfloat k1,k2,k3,p1,p2;
protected:

     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

private:



};

#endif // MYGLWIDGET_H
