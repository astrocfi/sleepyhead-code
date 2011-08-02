/*
 gGraphWindow Headers
 Copyright (c)2011 Mark Watkins <jedimark@users.sourceforge.net>
 License: GPL
*/

#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QGLContext>
#include <QGLWidget>
#include <QSplitter>
#include <QDateTime>
#include <QList>
using namespace std;
#include "graphlayer.h"
#include "glcommon.h"

#define MIN(a,b) (((a)<(b)) ? (a) : (b));
#define MAX(a,b) (((a)<(b)) ? (b) : (a));


class gLayer;

class gGraphWindow : public QGLWidget
{
    Q_OBJECT
public:
    //explicit gGraphWindow(QWidget *parent, const QString & title, QGLContext * context,Qt::WindowFlags f=0);
    explicit gGraphWindow(QWidget *parent, const QString & title, QGLWidget * shared,Qt::WindowFlags f=0);
    virtual ~gGraphWindow();

signals:
public slots:
public:
    QBitmap * RenderBitmap(int width,int height);

    virtual void paintGL();
    virtual void resizeGL(int width, int height);

    //virtual void OnMouseWheel(wxMouseEvent &event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
    //virtual void OnMouseRightDClick(wxMouseEvent * event);
    virtual void OnMouseLeftDown(QMouseEvent * event);
    virtual void OnMouseLeftRelease (QMouseEvent * event);
    virtual void OnMouseRightDown(QMouseEvent * event);
    virtual void OnMouseRightRelease(QMouseEvent * event);

    int GetScrX(void) const { return m_scrX; }
    int GetScrY(void) const { return m_scrY; }

    // For mouse to screen use only.. work in OpenGL points where possible
    const QString & Title(void ) { return m_title; }

      void SetMargins(int top, int right, int bottom, int left);  // OpenGL width of each corners margin

      int GetTopMargin(void) const { return m_marginTop; }
      int GetBottomMargin(void) const { return m_marginBottom; }
      int GetLeftMargin(void) const { return m_marginLeft; }
      int GetRightMargin(void) const { return m_marginRight; }

      void SetTopMargin(int i) { m_marginTop=i; }
      void SetBottomMargin(int i) { m_marginBottom=i; }
      void SetLeftMargin(int  i) { m_marginLeft=i; }
      void SetRightMargin(int i) { m_marginRight=i; }

      inline int Width() { return m_scrX-m_marginLeft-m_marginRight; }  // Width of OpenGL main drawing area
      inline int Height() { return m_scrY-m_marginTop-m_marginBottom; }   // Height of ""...

      void LinkZoom(gGraphWindow *g) { link_zoom.push_back(g); } // Linking graphs changes zoom behaviour..

      virtual qint64 MinX();
      virtual qint64 MaxX();
      virtual EventDataType MinY();
      virtual EventDataType MaxY();

      virtual void SetMinX(qint64 v);
      virtual void SetMaxX(qint64 v);
      virtual void SetMinY(EventDataType v);
      virtual void SetMaxY(EventDataType v);

      virtual void ResetBounds();
      virtual void SetXBounds(qint64 minx, qint64 maxx);
      virtual void ZoomX(double mult,int origin_px);

      virtual void ZoomXPixels(int x1, int x2);           // Zoom between two selected points on screen
      virtual void ZoomXPixels(int x1,int x2,qint64 &rx1,qint64 &rx2);

      virtual void MoveX(int i);                          // Move x bounds by i Pixels
      virtual void MoveX(int i,qint64 &min, qint64 & max);

      inline int x2p(qint64 x) {
          double xx=max_x-min_x;
          double wid=Width();
          double w=((wid/xx)*double(x-min_x));
          return w+GetLeftMargin();
      }
      inline qint64 p2x(int px) {
          double xx=max_x-min_x;
          double wx=px-GetLeftMargin();
          double ww=wx/Width();
          return min_x+(xx*ww);
      }
      inline int y2p(EventDataType y) {
          double yy=max_y-min_y;
          double h=(Height()/yy)*double(y-min_y);
          return h+GetBottomMargin();
      }
      inline EventDataType p2y(int py) {
          double yy=max_y-min_y;
          double hy=py-GetBottomMargin();
          double hh=hy/Height();
          return min_y+(yy*hh);
      }

      void Render(int scrx,int scry);

      void AddLayer(gLayer *l);

      qint64 max_x,min_x,max_y,min_y;
      qint64 rmax_x,rmin_x,rmax_y,rmin_y;

      void SetBlockZoom(bool b) { m_block_zoom=b; }
      bool BlockZoom() { return m_block_zoom; }
      QGLContext *gl_context;
      void SetGradientBackground(bool b) { m_gradient_background=b; }
      bool GradientBackground() { return m_gradient_background; }
      bool isEmpty();
      void SetSplitter(QSplitter *s) { splitter=s; }
      bool isDraggingGraph() { return m_dragGraph; }
      void setScry(int h) { m_scrY=h; }
  protected:
      void updateSelectionTime(qint64 span);
      //virtual void resizeEvent(QResizeEvent *);
      void initializeGL();
      QSplitter *splitter;
      QList<gGraphWindow *>link_zoom;

      bool m_block_zoom;
      bool m_drag_foobar;
      bool m_dragGraph;
      double m_foobar_pos,m_foobar_moved;

      bool m_gradient_background;
      QList<gLayer *> layers;
      QString m_title;
      int    m_scrX;
      int    m_scrY;
      QPoint m_mouseLClick,m_mouseRClick,m_mouseRClick_start;

      int m_marginTop, m_marginRight, m_marginBottom, m_marginLeft;

      QRect m_mouseRBrect,m_mouseRBlast;
      bool m_mouseLDown,m_mouseRDown,m_datarefresh;

      gLayer *foobar;
      gLayer *xaxis;
      gLayer *yaxis;
      gLayer *gtitle;

      QDateTime ti;
      gLayer *lastlayer;

};

#endif // GRAPHWINDOW_H
