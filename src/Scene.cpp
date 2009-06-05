
#include "Scene.moc"
#include "Ball.h"

#include <QMouseEvent>
#include <QTime>

#include <btBulletDynamicsCommon.h>

#include <math.h>
#include <float.h>

#include <QtDebug>

const qreal pi = 4.0 * atan(1.0);

#define ZNEAR 0.1
#define ZFAR 100.0

template <typename T>
inline void exp_mov_avg (T& avg, const T& new_val, const qreal& n)
{
    Q_ASSERT(n >= 1.0);
    qreal alpha = 2.0 / (n + 1.0);
    avg = (new_val * alpha) + (avg * (1.0 - alpha));
}

Scene::Scene () :
    dt(1.0/60.0),
    distance(10.0),
    zenith(pi/2.0),
    azimuth(-pi/2.0)
{
    connect(&timer, SIGNAL(timeout()), SLOT(update()));
}

Scene::~Scene ()
{
    foreach (Ball* ball, balls) {
        delete ball;
    }
    balls.clear();

    Ball::deinit();

    delete world;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

void Scene::init_physics ()
{
    int maxProxies = 1024;
    btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax( 10000,  10000,  10000);

    broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
    Q_ASSERT(broadphase);

    collisionConfiguration = new btDefaultCollisionConfiguration();
    Q_ASSERT(collisionConfiguration);

    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    Q_ASSERT(dispatcher);

    solver = new btSequentialImpulseConstraintSolver;
    Q_ASSERT(solver);

    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
                                        collisionConfiguration);
    Q_ASSERT(world);

    world->setGravity(btVector3(0, -10, 0));
}

void Scene::initializeGL ()
{
    init_physics();
    Ball::init();

    for (int i = 0; i < 5; i++) {
        balls << new Ball(this, i);
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe

    timer.start(1);

    GLuint gl_error = glGetError();
    if (gl_error != GL_NO_ERROR) {
        qFatal("OpenGL Error: %s", gluErrorString(gl_error));
    }

}

void Scene::resizeGL (int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,static_cast<double>(w)/static_cast<double>(h),ZNEAR,ZFAR);
}

void Scene::paintGL ()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    qreal sin_z = sin(zenith);
    qreal cos_z = cos(zenith);
    qreal sin_a = sin(-azimuth);
    qreal cos_a = cos(-azimuth);

    gluLookAt(distance * sin_z * cos_a,
              distance * cos_z,
              distance * sin_z * sin_a,
              0.0,  0.0,  0.0,
              0.0,  1.0,  0.0);

    glPushMatrix();
    glTranslatef(Ball::radius - (Ball::radius * balls.size()), 0, 0);
    foreach (Ball* ball, balls) {
        ball->draw();
    }
    glPopMatrix();

    GLuint gl_error = glGetError();
    if (gl_error != GL_NO_ERROR) {
        qFatal("OpenGL Error: %s", gluErrorString(gl_error));
    }

}

void Scene::mouseReleaseEvent (QMouseEvent* evt)
{
    last_mouse_pos = QPointF();
}

void Scene::mouseMoveEvent (QMouseEvent* evt)
{
    if (last_mouse_pos.isNull()) {
        last_mouse_pos = evt->posF();
        return;
    }
    QPointF diff = last_mouse_pos - evt->posF();
    last_mouse_pos = evt->posF();

    zenith  += diff.y() * 0.01;
    azimuth += diff.x() * 0.01;

    zenith = qBound(static_cast<double>(FLT_MIN), zenith, pi - FLT_MIN);

    updateGL();
}

void Scene::wheelEvent (QWheelEvent* evt)
{
    int degs = evt->delta() / 8;
    int steps = degs / 15;
    qreal diff = steps * -4.0;
    distance = qBound(ZNEAR, distance + diff, ZFAR);

    updateGL();
}

void Scene::update ()
{
    static QTime t;
    qreal real_dt = t.restart() * 0.001;
    if (real_dt > 1) {
        return;
    }
    exp_mov_avg(dt, real_dt, 240);

    int fps = qRound(1.0/dt);
    static int prev_fps = 0;
    if (prev_fps != fps) {
        prev_fps = fps;
        setWindowTitle(tr("Newton's Cradle by Mental Distortion (%1 fps)").arg(fps));
    }

    world->stepSimulation(dt, 10);
    updateGL();
}
