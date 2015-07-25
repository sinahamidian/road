#include "myglwidget.h"
#include <iostream>
using namespace std;

GLfloat abss(GLfloat a){
    if (a>=0)
        a;
    else
        a*-1;
}

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

//    glColor3f(0.0f,0.0f,0.0f);
//    int sum =0;
//    for(int i=0; i<conObjSize; i++)
//    {

//        glBegin(GL_LINES);
//        glVertex3f(-(imdots[1820+numberOfLines*2+sum].x/w)*2, -(imdots[1820+numberOfLines*2+sum].y/h)*2, 0);
//        glVertex3f(-(imdots[1820+numberOfLines*2+sum+contourSizes[i] -1].x/w)*2, -(imdots[1820+numberOfLines*2+sum+contourSizes[i] -1].y/h)*2, 0);
//        for(int j=0; j<contourSizes[i]-1; j++){
//            glVertex3f(-(imdots[1820+numberOfLines*2+sum+j].x/w)*2, -(imdots[1820+numberOfLines*2+sum+j].y/h)*2, 0);
//            glVertex3f(-(imdots[1820+numberOfLines*2+sum+j+1].x/w)*2, -(imdots[1820+numberOfLines*2+sum+j+1].y/h)*2, 0);
//        }
//        sum+= contourSizes[i];
//        glEnd();
//    }

    triangulation();

}

void MyGLWidget::resizeaa(int height, int width){
    w = width;
    h = height;
    if(((float)w/h)> ((float)ww/hh))
    {
        currentW = 1061;
        currentH = ((float)h/w)*1061;
    }
    else
    {
        currentH = 631;
        currentW = ((float)w/h)*631;
    }

    resizeGL(currentW,currentH);
}

void MyGLWidget::resizeGL(int width, int height){

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

void MyGLWidget::triangulation()
{
    int sum =0;
    vector <DOT*> temp;
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_TRIANGLES);
    for(int i=0; i<conObjSize; i++)
    {

//        glBegin(GL_LINES);
//        glVertex3f(-(imdots[1820+numberOfLines*2+sum].x/w)*2, -(imdots[1820+numberOfLines*2+sum].y/h)*2, 0);
//        glVertex3f(-(imdots[1820+numberOfLines*2+sum+contourSizes[i] -1].x/w)*2, -(imdots[1820+numberOfLines*2+sum+contourSizes[i] -1].y/h)*2, 0);
//        for(int j=0; j<contourSizes[i]-1; j++){
//            glVertex3f(-(imdots[1820+numberOfLines*2+sum+j].x/w)*2, -(imdots[1820+numberOfLines*2+sum+j].y/h)*2, 0);
//            glVertex3f(-(imdots[1820+numberOfLines*2+sum+j+1].x/w)*2, -(imdots[1820+numberOfLines*2+sum+j+1].y/h)*2, 0);
//        }
        DOT* t;
        for(int j=0; j<contourSizes[i]; j++)
        {
            t = new DOT;
            t->x = -(imdots[1820+numberOfLines*2+sum+j].x);
            t->y = -(imdots[1820+numberOfLines*2+sum+j].y);
            t->z = 0;
            temp.push_back(t);
            temp1.push_back(t);
        }

        while (temp.size() > 2 )
        {
            int j=0;
//            if (closeVertices(temp[temp.size()-1],temp[0],temp[1]))
//            {
//                temp.erase (temp.begin()+ 0);
//            }
            if(convex(temp[temp.size()-1]->x, temp[temp.size()-1]->y, temp[0]->x,temp[0]->y, temp[1]->x, temp[1]->y ))
            {
                if(!PointsInTriangles(0,&temp))
                {
                    glVertex3f(temp[temp.size()-1]->x*2/w, temp[temp.size()-1]->y*2/h, temp[temp.size()-1]->z);
                    glVertex3f(temp[0]->x*2/w,temp[0]->y*2/h,temp[0]->z);
                    glVertex3f(temp[1]->x*2/w,temp[1]->y*2/h,temp[1]->z);

                    temp.erase (temp.begin()+ 0);
                }
            }

            for (j=1; j<temp.size()-1; j++)
            {
//                if (closeVertices(temp[j-1],temp[j],temp[j+1]))
//                {
//                    temp.erase (temp.begin()+ j);
//                }
                if(convex(temp[j-1]->x, temp[j-1]->y, temp[j]->x,temp[j]->y, temp[j+1]->x, temp[j+1]->y ))
                {
                    if(!PointsInTriangles(j,&temp))
                    {
                        glVertex3f(temp[j-1]->x*2/w, temp[j-1]->y*2/h, temp[j-1]->z);
                        glVertex3f(temp[j]->x*2/w,temp[j]->y*2/h,temp[j]->z);
                        glVertex3f(temp[j+1]->x*2/w,temp[j+1]->y*2/h,temp[j+1]->z);

                        temp.erase (temp.begin()+ j);
                    }
                }
            }
            j=temp.size()-1;
//            if (closeVertices(temp[temp.size()-2],temp[temp.size()-1],temp[0]))
//            {
//                temp.erase (temp.begin()+ temp.size()-1);
//            }
            if(convex(temp[temp.size()-2]->x, temp[temp.size()-2]->y, temp[temp.size()-1]->x,temp[temp.size()-1]->y, temp[0]->x, temp[0]->y ))
            {
                if(!PointsInTriangles(temp.size()-1,&temp))
                {
                    glVertex3f(temp[temp.size()-2]->x*2/w, temp[temp.size()-2]->y*2/h, temp[temp.size()-2]->z);
                    glVertex3f(temp[temp.size()-1]->x*2/w,temp[temp.size()-1]->y*2/h,temp[temp.size()-1]->z);
                    glVertex3f(temp[0]->x*2/w,temp[0]->y*2/h,temp[0]->z);

                    temp.erase (temp.begin()+ temp.size()-1);
                }
            }

        }
        temp.clear();
        temp1.clear();

        sum+= contourSizes[i];
    }
    glEnd();
}

bool MyGLWidget::convex(double x1, double y1, double x2, double y2,
       double x3, double y3)
{
    if (area(x1, y1, x2, y2, x3, y3) <= 0)
        return true;
    else
        return false;
}



double MyGLWidget::area(double x1, double y1, double x2, double y2,
    double x3, double y3)
{
    double areaSum = 0;

    areaSum += x1 * (y3 - y2);
    areaSum += x2 * (y1 - y3);
    areaSum += x3 * (y2 - y1);

    return areaSum;
}

float MyGLWidget::sign (DOT* p1, DOT* p2, DOT* p3)
{
    return (p1->x - p3->x) * (p2->y - p3->y) - (p2->x - p3->x) * (p1->y - p3->y);
}

bool MyGLWidget::PointsInTriangles (int ptIndex, vector <DOT*> * points)
{
    bool b1, b2, b3;
    DOT* v1, *v2, *v3;
    if (ptIndex == 0)
    {
        v1 = (*points)[points->size()-1];
        v2 = (*points)[0];
        v3 = (*points)[1];
    }
    else if (ptIndex == points->size()-1)
    {
        v1 = (*points)[points->size() - 2];
        v2 = (*points)[points->size()-1];
        v3 = (*points)[0];
    }
    else
    {
        v1 = (*points)[ptIndex - 1];
        v2 = (*points)[ptIndex];
        v3 = (*points)[ptIndex + 1];
    }

    for(int i=0; i<temp1.size(); i++)
    {
        if(i != ptIndex){
            b1 = (sign(temp1[i], v1, v2) <= 0.0f);
            b2 = (sign(temp1[i], v2, v3) <= 0.0f);
            b3 = (sign(temp1[i], v3, v1) <= 0.0f);
            if (sign(temp1[i], v1, v2)== 0)
                continue;

            if ((b1 == b2) && (b2 == b3))
                return 1;
        }
    }
    return 0;
}

bool MyGLWidget::closeVertices(DOT* a, DOT* b, DOT* c){
    if ((abss(a->x - b->x) <= 0.01 && abss(a->y - b->y) <= 0.01)||(abss(c->x - b->x) <= 0.01 && abss(c->y - b->y) <= 0.01))
{
       return 1;
}
    return 0;
}
