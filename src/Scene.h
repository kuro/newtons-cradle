
#pragma once

#include <QGLWidget>
#include <QTimer>

class Ball;
class CubeMap;

class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class QSplashScreen;

class Scene : public QGLWidget
{
    Q_OBJECT

public:
    Scene ();
    virtual ~Scene ();

protected:
    void init_physics ();

    void initializeGL ();
    void resizeGL (int w, int h);
    void paintGL ();

    void mousePressEvent (QMouseEvent* evt);
    void mouseReleaseEvent (QMouseEvent* evt);
    void mouseMoveEvent (QMouseEvent* evt);
    void wheelEvent (QWheelEvent* evt);

protected slots:
    void update ();

public:
    btDiscreteDynamicsWorld* world;

protected:
    bool ready;
    qreal dt;

    QPointF last_mouse_pos;
    qreal distance;
    qreal zenith;
    qreal azimuth;

    QTimer timer;

    btAxisSweep3* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;

    QList<Ball*> balls;

    enum {
        DRAG_CAMERA,
        DRAG_BALL
    };
    int drag_mode;

    CubeMap* cubemap;
};
