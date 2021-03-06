#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <QtOpenGL/QtOpenGL>
#include <GL/glu.h>
#include <vector>

class MainWindow;

using namespace std;

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
    MainWindow *mw;
     explicit MyGLWidget(QWidget *parent = 0);
    MyGLWidget(int nl, GLfloat rw, GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4 , GLfloat x5, GLfloat roadX, GLfloat roadY);
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
     void resizeaa(int height, int width);
     void triangulation();
     bool convex(double x1, double y1, double x2, double y2,
                 double x3, double y3);
     double area(double x1, double y1, double x2, double y2,
                 double x3, double y3);
     float sign (DOT* p1, DOT* p2, DOT* p3);
     bool PointsInTriangles (int ptIndex, vector <DOT*> * points);
     bool closeVertices(DOT* a, DOT* b, DOT* c);
     vector <DOT*> temp1;


     int number;
     QVector3D xAxis, yAxis, zAxis;
     DOT* dots;
     DOT* dots0;
     DOT* imdots;
     int dotsSize, numberOfLines;
     GLfloat w, h, roadWidth;
     GLfloat ww, hh;
     GLfloat currentW, currentH;
     GLfloat xx, zz,  t,  p , e;
     GLfloat x,y,z ,camx,camy,camz,camteta,cambeta,camalpha;
     GLfloat F,sx,sy,cx,cy;
     GLfloat k1,k2,k3,p1,p2;
     GLfloat rotX,rotY,rotZ;
     GLfloat recx,recz,rect,recp, roadX, roadY;
     GLfloat pe,pw,ph;
     GLfloat NMx, NMy, NMz;//plkcoor
     CNT **contours;
     DOT **contourDOTS;
     int *contourSizes;
     int conObjSize;
     int contourAllSize;
     bool MinMaxFlag;

     //DOT plkcoor;


public slots:
     void mousePressEvent(QMouseEvent * event);
     void mouseDoubleClickEvent( QMouseEvent * event );
     void doubleClicked();

protected:

     void initializeGL();
     void resizeGL(int w, int h);
     void paintGL();

private:



};

#endif // MYGLWIDGET_H
