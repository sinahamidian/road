#include "myglwidget.h"
#include <iostream>

using namespace std;


MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    QSize a(1280,1024);

    this->resize(a);
//    glViewport(0,0,12,h);
    w = this->width();
    h = this->height();
}

MyGLWidget::MyGLWidget(int nl, GLfloat rw,GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4 , GLfloat x5){
    QSize a(1280,1024);

    this->resize(a);
//    glViewport(0,0,12,h);

    xAxis.setX(1);
    xAxis.setY(0);
    xAxis.setZ(0);

    yAxis.setX(0);
    yAxis.setY(1);
    yAxis.setZ(0);

    zAxis.setX(0);
    zAxis.setY(0);
    zAxis.setZ(1);

    xx = x1;
    zz = x2;
    t = x3;
    p = x4;
    e = x5;
    numberOfLines = nl;
    roadWidth = rw;
    w = this->width();
    h = this->height();
}

void MyGLWidget::initializeGL(){

    dotsSize = 1804+numberOfLines*2 +8;
    dots = new DOT [dotsSize];
    dots0 = new DOT [dotsSize];
    imdots = new DOT [dotsSize];


        dots[1800].z = 200;
        dots[1800].x = -roadWidth/2;
        dots[1800].y = -10;
        dots[1801].z = -200;//
        dots[1801].x = -roadWidth/2;
        dots[1801].y = -10;
        dots[1802].z = 200;
        dots[1802].x = roadWidth/2;
        dots[1802].y = -10;
        dots[1803].z = -200;//
        dots[1803].x = roadWidth/2;
        dots[1803].y = -10;

        GLfloat lineWidth = roadWidth/numberOfLines;
        for(int i=0; i<numberOfLines-1; i++)
        {
            dots[1804+i*2].z = 200;
            dots[1804+i*2].x = -roadWidth/2 + lineWidth*(i+1);
            dots[1804+i*2].y = -10;
            dots[1805+i*2].z = -200;//
            dots[1805+i*2].x = -roadWidth/2 + lineWidth*(i+1);
            dots[1805+i*2].y = -10;

        }

        //mokaab
        dots[1804+numberOfLines*2].x = xx;
        dots[1804+numberOfLines*2].y = -10;
        dots[1804+numberOfLines*2].z = -zz;

        dots[1804+numberOfLines*2+1].x = xx+p;
        dots[1804+numberOfLines*2+1].y = -10;
        dots[1804+numberOfLines*2+1].z = -zz;

        dots[1804+numberOfLines*2+2].x = xx+p;
        dots[1804+numberOfLines*2+2].y = -10+e;
        dots[1804+numberOfLines*2+2].z = -zz;

        dots[1804+numberOfLines*2+3].x = xx;
        dots[1804+numberOfLines*2+3].y = -10+e;
        dots[1804+numberOfLines*2+3].z = -zz;

        dots[1804+numberOfLines*2+4].x = xx;
        dots[1804+numberOfLines*2+4].y = -10;
        dots[1804+numberOfLines*2+4].z = -zz-t;

        dots[1804+numberOfLines*2+5].x = xx+p;
        dots[1804+numberOfLines*2+5].y = -10;
        dots[1804+numberOfLines*2+5].z = -zz-t;

        dots[1804+numberOfLines*2+6].x = xx+p;
        dots[1804+numberOfLines*2+6].y = -10+e;
        dots[1804+numberOfLines*2+6].z = -zz-t;

        dots[1804+numberOfLines*2+7].x = xx;
        dots[1804+numberOfLines*2+7].y = -10+e;
        dots[1804+numberOfLines*2+7].z = -zz-t;


        for(int i=0; i<200; i=i+2)
        {
            dots[i].z = 200;
            dots[i].x = -25 + (i)*0.25;
            dots[i].y = -10;

            dots[i+1].z = -200;//;
            dots[i+1].x = -25 + (i)*0.25;
            dots[i+1].y = -10;
        }

        for(int i=200; i<1800; i=i+2)
        {
            dots[i].z = -200+(i-200)*0.25;
            dots[i].x = -25 ;
            dots[i].y = -10;

            dots[i+1].z = -200+(i-200)*0.25;
            dots[i+1].x = +25;
            dots[i+1].y = -10;
        }




    F=0.1 * 1000; sx=(0.001)/(0.001); sy=0.001/0.001; cx=0; cy=0;
    k1 = 0; k2 = 0; k3 = 0;
    p1 = 0; p2 = 0;

    camx = 0; camy = 0; camz = 0;
    camalpha = 0; cambeta = 0; camteta = 0 ;

    camTrans();

    Intrinsics();
//    Distortion();

}

void MyGLWidget::Intrinsics(){
    for (int i=0; i<dotsSize; i++)
    {
        if (dots0[i].z >= 0)
        {
            GLfloat xdir, ydir, temp;
            if(i%2 == 0)
            {
                xdir = (dots0[i+1].x - dots0[i].x)/(dots0[i].z - dots0[i+1].z);
                ydir = (dots0[i+1].y - dots0[i].y)/(dots0[i].z - dots0[i+1].z);

                temp = dots0[i].z + 1;

                dots0[i].x += xdir*temp;
                dots0[i].y += ydir*temp;
            }
            else
            {
                xdir = (dots0[i-1].x - dots0[i].x)/(dots0[i].z - dots0[i-1].z);
                ydir = (dots0[i-1].y - dots0[i].y)/(dots0[i].z - dots0[i-1].z);

                temp = dots0[i].z + 1;

                dots0[i].x += xdir*temp;
                dots0[i].y += ydir*temp;

            }
            dots0[i].z = -1;

        }


        imdots[i].x = (F*sx*dots0[i].x)/(dots0[i].z);
        imdots[i].y = (F*sy*dots0[i].y)/(dots0[i].z);
        imdots[i].z = dots0[i].z;

    }
}

void MyGLWidget::Distortion(){
    GLfloat r;
    for (int i=0; i<dotsSize*2; i++)
    {
        r = sqrt(pow(imdots[i].x,2) + pow(imdots[i].y,2));
        imdots[i].x = imdots[i].x * (1 + k1*pow(r,2) + k2* pow(r,4) + k3* pow(r,6));
        imdots[i].y = imdots[i].y * (1 + k1*pow(r,2) + k2* pow(r,4) + k3* pow(r,6));

        imdots[i].x = imdots[i].x + (2*p1*imdots[i].y + p2*(pow(r,2)+2*pow(imdots[i].x,2)));
        imdots[i].y = imdots[i].y + (2*p2*imdots[i].x + p1*(pow(r,2)+2*pow(imdots[i].y,2)));

    }

}

void MyGLWidget::paintGL(){



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





//    GLfloat y;        // Storage for varying Y coordinate
//    GLfloat fSizes[2];      // Line width range metrics
//    GLfloat fCurrSize;      // Save current size

//    // Get line size metrics and save the smallest value
//    glGetFloatv(GL_LINE_WIDTH_RANGE,fSizes);
//    fCurrSize = fSizes[0];
//    fCurrSize += 2.0f;
//    glLineWidth(fCurrSize);
    glBegin(GL_LINES);

    glColor3f(0.0f,1.0f,0.0f);


    for(int i=0; i<1800; i=i+2){
            glVertex3f(-(imdots[i].x/w)*2, -(imdots[i].y/h)*2, 0);
            glVertex3f(-(imdots[i+1].x/w)*2, -(imdots[i+1].y/h)*2, 0);
    }


    glColor3f(1.0f,0.0f,0.0f);


    for(int i=1800; i<1804+numberOfLines*2; i=i+2){
            glVertex3f(-(imdots[i].x/w)*2, -(imdots[i].y/h)*2, 0);
            glVertex3f(-(imdots[i+1].x/w)*2, -(imdots[i+1].y/h)*2, 0);
    }
    glEnd();
    glBegin(GL_QUADS);

    if(camx<(xx+(p/2))){
        glColor3f(1.0f,1.0f,0.0f);

        glVertex3f(-(imdots[1804+numberOfLines*2].x/w)*2, -(imdots[1804+numberOfLines*2].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+3].x/w)*2, -(imdots[1804+numberOfLines*2+3].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+7].x/w)*2, -(imdots[1804+numberOfLines*2+7].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+4].x/w)*2, -(imdots[1804+numberOfLines*2+4].y/h)*2, 0);

        glColor3f(1.0f,1.0f,0.0f);

        glVertex3f(-(imdots[1804+numberOfLines*2+1].x/w)*2, -(imdots[1804+numberOfLines*2+1].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+2].x/w)*2, -(imdots[1804+numberOfLines*2+2].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+6].x/w)*2, -(imdots[1804+numberOfLines*2+6].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+5].x/w)*2, -(imdots[1804+numberOfLines*2+5].y/h)*2, 0);
    }
    else{
        glColor3f(1.0f,1.0f,0.0f);

        glVertex3f(-(imdots[1804+numberOfLines*2+1].x/w)*2, -(imdots[1804+numberOfLines*2+1].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+2].x/w)*2, -(imdots[1804+numberOfLines*2+2].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+6].x/w)*2, -(imdots[1804+numberOfLines*2+6].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+5].x/w)*2, -(imdots[1804+numberOfLines*2+5].y/h)*2, 0);

        glColor3f(1.0f,1.0f,0.0f);

        glVertex3f(-(imdots[1804+numberOfLines*2].x/w)*2, -(imdots[1804+numberOfLines*2].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+3].x/w)*2, -(imdots[1804+numberOfLines*2+3].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+7].x/w)*2, -(imdots[1804+numberOfLines*2+7].y/h)*2, 0);
        glVertex3f(-(imdots[1804+numberOfLines*2+4].x/w)*2, -(imdots[1804+numberOfLines*2+4].y/h)*2, 0);
    }

    glColor3f(0.0f,0.0f,1.0f);

    glVertex3f(-(imdots[1804+numberOfLines*2].x/w)*2, -(imdots[1804+numberOfLines*2].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+1].x/w)*2, -(imdots[1804+numberOfLines*2+1].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+2].x/w)*2, -(imdots[1804+numberOfLines*2+2].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+3].x/w)*2, -(imdots[1804+numberOfLines*2+3].y/h)*2, 0);

    glColor3f(0.0f,1.0f,1.0f);

    glVertex3f(-(imdots[1804+numberOfLines*2+3].x/w)*2, -(imdots[1804+numberOfLines*2+3].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+2].x/w)*2, -(imdots[1804+numberOfLines*2+2].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+6].x/w)*2, -(imdots[1804+numberOfLines*2+6].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+7].x/w)*2, -(imdots[1804+numberOfLines*2+7].y/h)*2, 0);

    glEnd();


}

void MyGLWidget::resizeGL(int width, int height){
    w = width;
    h = height;
         glViewport(0, 0 , width, height);

         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
     #ifdef QT_OPENGL_ES_1
         glOrthof(-1, +1, -1, +1, -1, 1);
     #else
         glOrtho(-1, +1, -1, +1, -1, 1);
     #endif
         glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::rotateAroundAnAxis(GLfloat angle, QVector3D* in, QVector3D* Axis)
{
    GLfloat sint = sin(angle);
    GLfloat cost = cos(angle);
    QVector3D temp = *in;
    in->setX((cost+pow(Axis->x(),2)*(1-cost))*temp.x() + (Axis->x()*Axis->y()*(1-cost) - Axis->z()*sint)*temp.y() + (Axis->x()*Axis->z()*(1-cost)+Axis->y()*sint)*temp.z());
    in->setY((Axis->y()*Axis->x()*(1-cost)+ Axis->z()*sint)*temp.x() + (cost+pow(Axis->y(),2)*(1-cost))*temp.y() + (Axis->y()*Axis->z()*(1-cost)-Axis->x()*sint)*temp.z());
    in->setZ((Axis->z()*Axis->x()*(1-cost)-Axis->y()*sint)*temp.x() + (Axis->z()*Axis->y()*(1-cost)+Axis->x()*sint)*temp.y() + (cost+pow(Axis->z(),2)*(1-cost))*temp.z());
}

void MyGLWidget::camTrans(){

    QVector3D xAx = xAxis;
    QVector3D yAx = yAxis;
    QVector3D zAx = zAxis;


    rotateAroundAnAxis(camalpha,&xAx,&xAxis);
    rotateAroundAnAxis(camalpha,&yAx,&xAxis);
    rotateAroundAnAxis(camalpha,&zAx,&xAxis);

    QVector3D yyy = yAx;

    rotateAroundAnAxis(cambeta,&xAx,&yyy);
    rotateAroundAnAxis(cambeta,&yAx,&yyy);
    rotateAroundAnAxis(cambeta,&zAx,&yyy);

    QVector3D zzz = zAx;

    rotateAroundAnAxis(camteta,&xAx,&zzz);
    rotateAroundAnAxis(camteta,&yAx,&zzz);
    rotateAroundAnAxis(camteta,&zAx,&zzz);





    for(int i=0; i<dotsSize; i++)
    {
        dots0[i].x = dots[i].x - camx;
        dots0[i].y = dots[i].y - camy;
        dots0[i].z = dots[i].z - camz;


//        dots0[i].x = x*cost*cosb + (sint*cosa - sina*cost*sinb)*y + (sina*sint + cost*cosa*sinb)*z;
//        dots0[i].y = -sint*cosb*x + (cost*cosa + sina*sinb*sint)*y + (sina*cost - cosa*sint*sinb)*z;
//        dots0[i].z = -sinb*x - sina*cosb*y + cosb*cosa*z;
        QVector3D a;
        a.setX(dots0[i].x);
        a.setY(dots0[i].y);
        a.setZ(dots0[i].z);

        rotateAroundAnAxis(camalpha, &a, &xAxis);
        rotateAroundAnAxis(cambeta, &a, &yyy);
        rotateAroundAnAxis(camteta, &a, &zzz);

        dots0[i].x = a.x();
        dots0[i].y = a.y();
        dots0[i].z = a.z();

    }


}

void MyGLWidget::clear(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
