
#pragma once

#include <QGLWidget>
#include <QTimer>

#include <Cg/cg.h>

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

    struct cg
    {
        CGcontext context;
        CGprofile vertex_profile;
        CGprofile fragment_profile;
    } cg;

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
    GLuint sky_dlist;


    CGprogram sky_program;
};
