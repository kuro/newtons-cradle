
#pragma once

#include <QGLWidget>

class Scene : public QGLWidget
{
    Q_OBJECT

public:
    Scene ();
    virtual ~Scene ();

protected:
    void initializeGL ();
    void resizeGL (int w, int h);
    void paintGL ();

    void mouseReleaseEvent (QMouseEvent* evt);
    void mouseMoveEvent (QMouseEvent* evt);
    void wheelEvent (QWheelEvent* evt);

    QPointF last_mouse_pos;

protected:
    qreal distance;
    qreal zenith;
    qreal azimuth;

};
