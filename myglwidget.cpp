#include "myglwidget.h"
#include <iostream>
using namespace std;


MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    QSize a(640,512);

    this->resize(a);
//    glViewport(0,0,12,h);
    w = this->width();
    h = this->height();
    rotX = rotZ = rotY = 0;
}

MyGLWidget::MyGLWidget(int nl, GLfloat rw,GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4 , GLfloat x5){
    w = 640;
    h = 512;
    QSize a(w,h);

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

    pe = 0.4;
    pw = 0.5;
    ph = 0.15;
}

void MyGLWidget::initializeGL(){

    rect = 1;
    recp = 1;
    recx = 0;
    recz = 20;
    makeTheContours();


    dotsSize = 1804+numberOfLines*2 +8 +4 +4 +contourAllSize;
    dots = new DOT [dotsSize];
    dots0 = new DOT [dotsSize];
    imdots = new DOT [dotsSize];

    makeConDOTS();


        dots[1800].z = 200;
        dots[1800].x = -roadWidth/2;
        dots[1800].y = 0;
        dots[1801].z = -200;//
        dots[1801].x = -roadWidth/2;
        dots[1801].y = 0;
        dots[1802].z = 200;
        dots[1802].x = roadWidth/2;
        dots[1802].y = 0;
        dots[1803].z = -200;//
        dots[1803].x = roadWidth/2;
        dots[1803].y = 0;

        GLfloat lineWidth = roadWidth/numberOfLines;
        for(int i=0; i<numberOfLines-1; i++)
        {
            dots[1804+i*2].z = 200;
            dots[1804+i*2].x = -roadWidth/2 + lineWidth*(i+1);
            dots[1804+i*2].y = 0;
            dots[1805+i*2].z = -200;//
            dots[1805+i*2].x = -roadWidth/2 + lineWidth*(i+1);
            dots[1805+i*2].y = 0;

        }

        //mokaab
        makeTheCar();

        makeTheRec();

        makeThePLK();

        for(int i=0; i<200; i=i+2)
        {
            dots[i].z = 200;
            dots[i].x = -25 + (i)*0.25;
            dots[i].y = 0;

            dots[i+1].z = -200;//;
            dots[i+1].x = -25 + (i)*0.25;
            dots[i+1].y = 0;
        }

        for(int i=200; i<1800; i=i+2)
        {
            dots[i].z = -200+(i-200)*0.25;
            dots[i].x = -25 ;
            dots[i].y = 0;

            dots[i+1].z = -200+(i-200)*0.25;
            dots[i+1].x = +25;
            dots[i+1].y = 0;
        }




    F=0.1 * 1000; sx=(0.001)/(0.001); sy=0.001/0.001; cx=0; cy=0;
    k1 = 0; k2 = 0; k3 = 0;
    p1 = 0; p2 = 0;

    camx = 0; camy = -10; camz = 0;
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


        imdots[i].x = ((F*sx*dots0[i].x)/(dots0[i].z))+cx;
        imdots[i].y = ((F*sy*dots0[i].y)/(dots0[i].z))+cy;
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

    if(-camx<(xx+(p/2))){
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

    glBegin(GL_LINES);//mostatil
    glColor3f(0.3f,0.7f,0.6f);
    glVertex3f(-(imdots[1804+numberOfLines*2+8].x/w)*2, -(imdots[1804+numberOfLines*2+8].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+9].x/w)*2, -(imdots[1804+numberOfLines*2+9].y/h)*2, 0);

    glVertex3f(-(imdots[1804+numberOfLines*2+9].x/w)*2, -(imdots[1804+numberOfLines*2+9].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+10].x/w)*2, -(imdots[1804+numberOfLines*2+10].y/h)*2, 0);

    glVertex3f(-(imdots[1804+numberOfLines*2+10].x/w)*2, -(imdots[1804+numberOfLines*2+10].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+11].x/w)*2, -(imdots[1804+numberOfLines*2+11].y/h)*2, 0);

    glVertex3f(-(imdots[1804+numberOfLines*2+11].x/w)*2, -(imdots[1804+numberOfLines*2+11].y/h)*2, 0);
    glVertex3f(-(imdots[1804+numberOfLines*2+8].x/w)*2, -(imdots[1804+numberOfLines*2+8].y/h)*2, 0);
    glEnd();

    glBegin(GL_QUADS);;//pelak
    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f(-(imdots[1816+numberOfLines*2+0].x/w)*2, -(imdots[1816+numberOfLines*2+0].y/h)*2, 0);
    glVertex3f(-(imdots[1816+numberOfLines*2+1].x/w)*2, -(imdots[1816+numberOfLines*2+1].y/h)*2, 0);
    glVertex3f(-(imdots[1816+numberOfLines*2+2].x/w)*2, -(imdots[1816+numberOfLines*2+2].y/h)*2, 0);
    glVertex3f(-(imdots[1816+numberOfLines*2+3].x/w)*2, -(imdots[1816+numberOfLines*2+3].y/h)*2, 0);
    glEnd();

    glColor3f(0.0f,0.0f,0.0f);
    int sum =0;
    for(int i=0; i<conObjSize; i++)
    {

        glBegin(GL_POLYGON);
        glVertex3f(-(imdots[1820+numberOfLines*2+sum].x/w)*2, -(imdots[1820+numberOfLines*2+sum].y/h)*2, 0);
        glVertex3f(-(imdots[1820+numberOfLines*2+sum+contourSizes[i] -1].x/w)*2, -(imdots[1820+numberOfLines*2+sum+contourSizes[i] -1].y/h)*2, 0);
        for(int j=0; j<contourSizes[i]-1; j++){
            glVertex3f(-(imdots[1820+numberOfLines*2+sum+j].x/w)*2, -(imdots[1820+numberOfLines*2+sum+j].y/h)*2, 0);
            glVertex3f(-(imdots[1820+numberOfLines*2+sum+j+1].x/w)*2, -(imdots[1820+numberOfLines*2+sum+j+1].y/h)*2, 0);
        }
        sum+= contourSizes[i];
        glEnd();
    }


}

void MyGLWidget::resizeaa(int h, int w){
    resizeGL(w,h);
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
        dots0[i].x = dots[i].x + camx;
        dots0[i].y = dots[i].y + camy;
        dots0[i].z = dots[i].z + camz;


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

void MyGLWidget::makeTheCar(){
    dots[1804+numberOfLines*2].x = xx-camx;
    dots[1804+numberOfLines*2].y = 0;
    dots[1804+numberOfLines*2].z = -zz+camz;

    dots[1804+numberOfLines*2+1].x = xx+p-camx;
    dots[1804+numberOfLines*2+1].y = 0;
    dots[1804+numberOfLines*2+1].z = -zz+camz;

    dots[1804+numberOfLines*2+2].x = xx+p-camx;
    dots[1804+numberOfLines*2+2].y = 0+e;
    dots[1804+numberOfLines*2+2].z = -zz+camz;

    dots[1804+numberOfLines*2+3].x = xx-camx;
    dots[1804+numberOfLines*2+3].y = 0+e;
    dots[1804+numberOfLines*2+3].z = -zz+camz;

    dots[1804+numberOfLines*2+4].x = xx-camx;
    dots[1804+numberOfLines*2+4].y = 0;
    dots[1804+numberOfLines*2+4].z = -zz-t+camz;

    dots[1804+numberOfLines*2+5].x = xx+p-camx;
    dots[1804+numberOfLines*2+5].y = 0;
    dots[1804+numberOfLines*2+5].z = -zz-t+camz;

    dots[1804+numberOfLines*2+6].x = xx+p-camx;
    dots[1804+numberOfLines*2+6].y = 0+e;
    dots[1804+numberOfLines*2+6].z = -zz-t+camz;

    dots[1804+numberOfLines*2+7].x = xx-camx;
    dots[1804+numberOfLines*2+7].y = 0+e;
    dots[1804+numberOfLines*2+7].z = -zz-t+camz;

}

void MyGLWidget::makeTheRec(){
    dots[1804+numberOfLines*2 +8].x = recx-camx;
    dots[1804+numberOfLines*2 +8].y = 0;
    dots[1804+numberOfLines*2 +8].z = -recz+camz;

    dots[1804+numberOfLines*2 +9].x = recx-camx + recp;
    dots[1804+numberOfLines*2 +9].y = 0;
    dots[1804+numberOfLines*2 +9].z = -recz+camz;

    dots[1804+numberOfLines*2 +10].x = recx-camx + recp;
    dots[1804+numberOfLines*2 +10].y = 0;
    dots[1804+numberOfLines*2 +10].z = -recz+camz - rect;

    dots[1804+numberOfLines*2 +11].x = recx-camx;
    dots[1804+numberOfLines*2 +11].y = 0;
    dots[1804+numberOfLines*2 +11].z = -recz+camz - rect;

}

void MyGLWidget::makeThePLK(){
    dots[1816+numberOfLines*2].x = xx + (float)(p-pw)/2 -camx;
    dots[1816+numberOfLines*2].y = pe;
    dots[1816+numberOfLines*2].z = -zz +camz;

    dots[1816+numberOfLines*2+1].x = xx + (float)(p-pw)/2 + pw-camx;
    dots[1816+numberOfLines*2+1].y = pe;
    dots[1816+numberOfLines*2+1].z = -zz +camz;

    dots[1816+numberOfLines*2+2].x = xx + (float)(p-pw)/2 + pw-camx;
    dots[1816+numberOfLines*2+2].y = pe+ph;
    dots[1816+numberOfLines*2+2].z = -zz+camz;

    dots[1816+numberOfLines*2+3].x = xx + (float)(p-pw)/2 -camx;
    dots[1816+numberOfLines*2+3].y = pe+ph;
    dots[1816+numberOfLines*2+3].z = -zz+camz;

    NMx = xx + (float)(p-pw)/2 -camx;
    NMy = pe+ph;
    NMz = -zz+camz;
}

void MyGLWidget::makeTheContours(){
    QString fileaddr;

    fileaddr = QFileDialog::getOpenFileName(this, tr("Contours"), "", tr("Text Files (*.txt)"));
    QFile file(fileaddr);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString word;
    int maxI = 0;
    int temp , temp1=0 , temp2=0;
    int t;
    while(!in.atEnd())
    {
        in>>word;
        if (word.length()>15 && word.left(15) == "plate_contours["){
            temp = word.mid(15,1).toInt();
            if(word[16] != ']')
            {
                temp = word.mid(15,2).toInt();
            }
            if (temp>maxI)
                maxI = temp;
        }

    }
    maxI++;
    contours = new CNT* [maxI];
    contourSizes = new int [maxI];
    for(int i=0; i<maxI; i++)
        contourSizes[i] = 0;

    contourDOTS = new DOT* [maxI];

    in.seek(0);
    while(!in.atEnd())
    {
        in>>word;
        if (word.length()>24 && word.left(15) == "plate_contours["){
            temp = word.mid(15,1).toInt();
            t = 15;
            if(word[16] != ']')
            {
                temp = word.mid(15,2).toInt();
                t = 16;
            }
            t = t+10;
            int ll = 0;
            while(word[t+ll]!=')')
            {
                ll++;
            }
            temp1 = word.mid(t,ll).toInt();
            contourSizes[temp] = temp1;
            contours[temp] = new CNT [temp1];
            contourDOTS[temp] = new DOT [temp1];
        }
    }

    int iter;
    in.seek(0);
    while(!in.atEnd())
     {
        in>>word;
        if (word.length()>15 && word.left(15) == "plate_contours["){
            temp = word.mid(15,1).toInt();
            t = 15;
            if(word[16] != ']')
            {
                temp = word.mid(15,2).toInt();
                t = 16;
            }
            t = t+2;

            if(word[t] != '[')
                continue;


            t++;
            int ll = 0;
            while(word[t+ll]!=']')
            {
                ll++;
            }
            iter = word.mid(t,ll).toInt();

            in>>word;
            in>>word;
            t=10;
            ll=0;
            while(word[t+ll]!=',')
            {
                ll++;
            }
            temp1 = word.mid(t,ll).toInt();

            t=0; ll=0;
            in>>word;
            while(word[t+ll]!=')')
            {
                ll++;
            }
            temp2 = word.mid(t,ll).toInt();

            contours[temp][iter].x = temp1;
            contours[temp][iter].y = temp2;
        }

    }
    int sum=0;
    conObjSize = maxI;
        for(int i=0;i<conObjSize;i++)
        {
            sum+= contourSizes[i];
        }
    contourAllSize = sum;
}

void MyGLWidget::makeConDOTS(){
    int sum=0;
    for(int i=0; i<conObjSize; i++)
    {
        for(int j=0; j<contourSizes[i]; j++){
            dots[1820+numberOfLines*2 +sum+j].x = NMx+(0.001*(float)contours[i][j].x);
            dots[1820+numberOfLines*2 +sum+j].y = NMy-(0.001*(float)contours[i][j].y);
            dots[1820+numberOfLines*2 +sum+j].z = NMz;
        }
        sum+= contourSizes[i];
    }
}
