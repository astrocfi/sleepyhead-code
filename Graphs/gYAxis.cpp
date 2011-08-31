/*
 gYAxis Implementation
 Copyright (c)2011 Mark Watkins <jedimark@users.sourceforge.net>
 License: GPL
*/

#include <math.h>
#include <QDebug>
#include "gYAxis.h"

gYSpacer::gYSpacer(int spacer) :Layer(EmptyChannel)
{
};

gXGrid::gXGrid(QColor col)
    :Layer(EmptyChannel)
{
    m_major_color=QColor(190,190,190,64);
    m_minor_color=QColor(220,220,220,64);
    m_show_major_lines=true;
    m_show_minor_lines=true;

    addGLBuf(majorvert=new GLBuffer(m_major_color));
    addGLBuf(minorvert=new GLBuffer(m_minor_color));
}
gXGrid::~gXGrid()
{
    delete minorvert;
    delete majorvert;
}
void gXGrid::paint(gGraph & w,int left,int top, int width, int height)
{
    int x,y;

    double miny=w.min_y;
    double maxy=w.max_y;

    if (miny<0) {
        miny=-MAX(fabs(miny),fabs(maxy));
    }
    double dy=maxy-miny;
    if (dy<=0) {
        if ((maxy==0) && (miny==0))
            return;
        //miny=miny;
        maxy++;
        dy=1;
    }


    int m;
    if (maxy>500) {
        m=ceil(maxy/100.0);
        maxy=m*100;
        m=floor(miny/100.0);
        miny=m*100;
    } else if (maxy>150) {
        m=ceil(maxy/50.0);
        maxy=m*50;
        m=floor(miny/50.0);
        miny=m*50;
    } else if (maxy>80) {
        m=ceil(maxy/20.0);
        maxy=m*20;
        m=floor(miny/20.0);
        miny=m*20;
    } else if (maxy>30) {
        m=ceil(maxy/10.0);
        maxy=m*10;
        m=floor(miny/10.0);
        miny=m*10;
    } else if (maxy>5) {
        m=ceil(maxy/5.0);
        maxy=m*5;
        m=floor(miny/5.0);
        miny=m*5;
    }
    if (height<0) return;

    QString fd="0";
    GetTextExtent(fd,x,y);

    double max_yticks=round(height / (y+15.0)); // plus spacing between lines
    double yt=1/max_yticks;

    double mxy=MAX(fabs(maxy),fabs(miny));
    double mny=miny;
    if (miny<0) {
        mny=-mxy;
    }
    double rxy=mxy-mny;
    double ymult=height/rxy;

    double min_ytick=rxy*yt;

    float ty,h;

    /*qint32 vertcnt=0;
    GLshort * vertarray=(GLshort *)vertex_array[0];
    qint32 minorvertcnt=0;
    GLshort * minorvertarray=(GLshort *)vertex_array[1];
    qint32 majorvertcnt=0;
    GLshort * majorvertarray=(GLshort *)vertex_array[2];

    if ((vertarray==NULL) || (minorvertarray==NULL) || (majorvertarray==NULL)) {
        qWarning() << "gXGrid::Paint()  VertArray==NULL";
        return;
    } */

    if (min_ytick<=0) {
        qDebug() << "min_ytick error in gXGrid::paint()";
        return;
    }
    if (min_ytick>=1000000) {
        min_ytick=100;
    }

    double q=((maxy-(miny+(min_ytick/2.0)))/min_ytick)*4;
    //if (q>=maxverts) {
    //    qDebug() << "Would exeed maxverts. Should be another two bounds exceeded messages after this. (I can do a minor optimisation by disabling the other checks if this turns out to be consistent)" << q << maxverts;
    //}

    for (double i=miny; i<=maxy+min_ytick-0.00001; i+=min_ytick) {
        ty=(i - miny) * ymult;
        h=top+height-ty;
        if (m_show_major_lines && (i > miny)) {
            majorvert->add(left,h);
            majorvert->add(left+width,h);
        }
        double z=(min_ytick/4)*ymult;
        double g=h;
        for (int i=0;i<3;i++) {
            g+=z;
            if (g>top+height) break;
            //if (vertcnt>=maxverts) {
              //  qWarning() << "vertarray bounds exceeded in gYAxis for " << w.title() << "graph" << "MinY =" <<miny << "MaxY =" << maxy << "min_ytick=" <<min_ytick;
//                break;
  //          }
            if (m_show_minor_lines) {// && (i > miny)) {
                minorvert->add(left,g);
                minorvert->add(left+width,g);
            }
            if (minorvert->full()) {
                break;
            }
        }
        if (majorvert->full() || minorvert->full()) {
            qWarning() << "vertarray bounds exceeded in gYAxis for " << w.title() << "graph" << "MinY =" <<miny << "MaxY =" << maxy << "min_ytick=" <<min_ytick;
            break;
        }
    }

    // Draw the lines & ticks
    // Turn on blending??

    //glLineWidth(1);
    //majorvert->draw();
    //minorvert->draw();
    /*
    glEnableClientState(GL_VERTEX_ARRAY);
    w.qglColor(m_minor_color);
    glVertexPointer(2, GL_SHORT, 0, minorvertarray);
    glDrawArrays(GL_LINES, 0, minorvertcnt>>1);
    w.qglColor(m_major_color);
    glVertexPointer(2, GL_SHORT, 0, majorvertarray);
    glDrawArrays(GL_LINES, 0, majorvertcnt>>1);
    glDisableClientState(GL_VERTEX_ARRAY); // deactivate vertex arrays after drawing
    */
}



gYAxis::gYAxis(QColor col)
:Layer(EmptyChannel)
{
    m_line_color=col;
    m_text_color=col;

    m_yaxis_scale=1;
    addGLBuf(vertarray=new GLBuffer(m_line_color));
}
gYAxis::~gYAxis()
{
    delete vertarray;
}
void gYAxis::paint(gGraph & w,int left,int top, int width, int height)
{
    int x,y;
    int labelW=0;

    double miny=w.min_y;
    double maxy=w.max_y;

    if (miny<0) {
        miny=-MAX(fabs(miny),fabs(maxy));
    }
    double dy=maxy-miny;
    if (dy<=0) {
        if ((maxy==0) && (miny==0))
            return;
        //miny=miny;
        maxy++;
        dy=1;
    }


    int m;
    if (maxy>500) {
        m=ceil(maxy/100.0);
        maxy=m*100;
        m=floor(miny/100.0);
        miny=m*100;
    } else if (maxy>150) {
        m=ceil(maxy/50.0);
        maxy=m*50;
        m=floor(miny/50.0);
        miny=m*50;
    } else if (maxy>80) {
        m=ceil(maxy/20.0);
        maxy=m*20;
        m=floor(miny/20.0);
        miny=m*20;
    } else if (maxy>30) {
        m=ceil(maxy/10.0);
        maxy=m*10;
        m=floor(miny/10.0);
        miny=m*10;
    } else if (maxy>5) {
        m=ceil(maxy/5.0);
        maxy=m*5;
        m=floor(miny/5.0);
        miny=m*5;
    } else {
        //maxy=ceil(maxy);
        //if (maxy<1) maxy=1;
        //miny=floor(miny);
        //if (miny<1) miny=0;

    }

    //if ((w.max_x-w.min_x)==0)
    //    return;

    if (height<0) return;

    QString fd="0";
    GetTextExtent(fd,x,y);

    double max_yticks=round(height / (y+15.0)); // plus spacing between lines
    double yt=1/max_yticks;

    double mxy=MAX(fabs(maxy),fabs(miny));
    double mny=miny;
    if (miny<0) {
        mny=-mxy;
    } else {
    }
    //double mny=MIN(fabs(maxy),fabs(miny));
    //if (miny<0) mny=-mny;
    //if (maxy<0) mxy=-mxy;
    //mny=miny;
    //mxy=maxy;

    double rxy=mxy-mny;
    double ymult=height/rxy;

    double min_ytick=rxy*yt;

    float ty,h;

    /*qint32 vertcnt=0;
    GLshort * vertarray=(GLshort *)vertex_array[0];
    qint32 minorvertcnt=0;
    GLshort * minorvertarray=(GLshort *)vertex_array[1];
    qint32 majorvertcnt=0;
    GLshort * majorvertarray=(GLshort *)vertex_array[2];

    if ((vertarray==NULL) || (minorvertarray==NULL) || (majorvertarray==NULL)) {
        qWarning() << "gYAxis::Plot()  VertArray==NULL";
        return;
    } */

    if (min_ytick<=0) {
        qDebug() << "min_ytick error in gYAxis::Plot()";
        return;
    }
    if (min_ytick>=1000000) {
        min_ytick=100;
    }

    double q=((maxy-(miny+(min_ytick/2.0)))/min_ytick)*4;
    /*if (q>=maxverts) {
        qDebug() << "Would exeed maxverts. Should be another two bounds exceeded messages after this. (I can do a minor optimisation by disabling the other checks if this turns out to be consistent)" << q << maxverts;
    }*/

    w.qglColor(m_text_color);
    for (double i=miny; i<=maxy+min_ytick-0.00001; i+=min_ytick) {
        ty=(i - miny) * ymult;
        fd=Format(i*m_yaxis_scale); // Override this as a function.
        GetTextExtent(fd,x,y);
        if (x>labelW) labelW=x;
        h=top+height-ty;
        //w.renderText(start_px-12-x,scry-(h-(y/2.0)),fd);
        //DrawText(w,fd,left+width-8-x,(h+(y/2.0)),0,m_text_color);
        w.renderText(fd,left+width-8-x,(h+(y/2.0)),0,m_text_color);

        vertarray->add(left+width-4,h);
        vertarray->add(left+width,h);

        double z=(min_ytick/4)*ymult;
        double g=h;
        for (int i=0;i<3;i++) {
            g+=z;
            if (g>top+height) break;
            vertarray->add(left+width-3,g);
            vertarray->add(left+width,g);
            if (vertarray->full()) {
                qWarning() << "vertarray bounds exceeded in gYAxis for " << w.title() << "graph" << "MinY =" <<miny << "MaxY =" << maxy << "min_ytick=" <<min_ytick;
                break;
            }
        }
        if (vertarray->full()) {
            qWarning() << "vertarray bounds exceeded in gYAxis for " << w.title() << "graph" << "MinY =" <<miny << "MaxY =" << maxy << "min_ytick=" <<min_ytick;
            break;
        }
    }
    //vertarray->draw();
}

