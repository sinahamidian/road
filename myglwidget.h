#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL/QtOpenGL>
#include <GL/glu.h>



struct DOT{
    GLfloat x,y,z;
};

struct CNT{
    int x, y;
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
     void makeTheCar();
     void makeTheRec();
     void makeThePLK();
     void makeTheContours();
     void makeConDOTS();
     void resizeaa(int h, int w);


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
     GLfloat rotX,rotY,rotZ;
     GLfloat recx,recz,rect,recp;
     GLfloat pe,pw,ph;
     GLfloat NMx, NMy, NMz;//plkcoor
     CNT **contours;
     DOT **contourDOTS;
     int *contourSizes;
     int conObjSize;
     int contourAllSize;
     //DOT plkcoor;
protected:

     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

private:



};

#endif // MYGLWIDGET_H
