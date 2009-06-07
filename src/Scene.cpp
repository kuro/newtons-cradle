
#include <GL/glew.h>

#include "Scene.moc"
#include "Ball.h"
#include "CubeMap.h"

#include <QMouseEvent>
#include <QTime>
#include <QSplashScreen>
#include <QCoreApplication>

#include <btBulletDynamicsCommon.h>

#include <Cg/cgGL.h>

#include <math.h>
#include <float.h>

#include <QtDebug>

const qreal pi = 4.0 * atan(1.0);

extern QSplashScreen* splash;

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
    ready(false),
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
    static bool once = false;

    // calling processEvents() results in initializeGL() being called again
    if (once) {
        return;
    }
    once = true;

    splash->showMessage("initializing", Qt::AlignLeft, Qt::cyan);
    qApp->processEvents();

    glewInit();

    splash->showMessage("physics", Qt::AlignLeft, Qt::cyan);
    qApp->processEvents();

    init_physics();
    Ball::init();

    int count = 5;
    for (int i = 0; i < count; i++) {
        balls << new Ball(this, i, count);
    }

    splash->showMessage("graphics", Qt::AlignLeft, Qt::cyan);
    qApp->processEvents();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    //glEnable(GL_CULL_FACE);
    glPolygonMode(GL_BACK, GL_LINE);  // wireframe

    timer.start(1);

    GLuint gl_error = glGetError();
    if (gl_error != GL_NO_ERROR) {
        qFatal("OpenGL Error: %s", gluErrorString(gl_error));
    }

    splash->showMessage("loading environment map", Qt::AlignLeft, Qt::cyan);
    qApp->processEvents();

    glEnable(GL_TEXTURE_CUBE_MAP);
    cubemap = new CubeMap("media:cubemaps/SwedishRoyalCastle.exr");

    ready = true;
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
    if (!ready) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    qreal sin_z = sin(zenith);
    qreal cos_z = cos(zenith);
    qreal sin_a = sin(-azimuth);
    qreal cos_a = cos(-azimuth);

    qreal cam_x = distance * sin_z * cos_a;
    qreal cam_y = distance * cos_z        ;
    qreal cam_z = distance * sin_z * sin_a;

    gluLookAt(cam_x, cam_y, cam_z,
              0.0,  0.0,  0.0,
              0.0,  1.0,  0.0);

    cgGLEnableProfile(Ball::cg.vertex_profile);
    cgGLEnableProfile(Ball::cg.fragment_profile);
    cgGLBindProgram(Ball::cg.program);

    cgSetParameter3f(Ball::cg.eye, cam_x, cam_y, cam_z);

    foreach (Ball* ball, balls) {
        ball->draw();
    }

    cgGLDisableProfile(Ball::cg.vertex_profile);
    cgGLDisableProfile(Ball::cg.fragment_profile);

    GLuint gl_error = glGetError();
    if (gl_error != GL_NO_ERROR) {
        qFatal("OpenGL Error: %s", gluErrorString(gl_error));
    }

}

void Scene::mousePressEvent (QMouseEvent* evt)
{
    if (evt->modifiers() & Qt::AltModifier) {
        drag_mode = DRAG_CAMERA;
        return;
    }

    GLdouble x0, y0, z0, x1, y1, z1;
    GLdouble model[16];
    GLdouble proj[16];
    GLint view[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);

    qreal proper_x = evt->posF().x();
    qreal proper_y = view[3] - evt->posF().y() - 1;

    gluUnProject(proper_x, proper_y, 0.0, model, proj, view, &x0, &y0, &z0);
    gluUnProject(proper_x, proper_y, 1.0, model, proj, view, &x1, &y1, &z1);

    btVector3 v0 (x0, y0, z0);
    btVector3 v1 (x1, y1, z1);

    btCollisionWorld::ClosestRayResultCallback ray_callback(v0, v1);
    world->rayTest(v0, v1, ray_callback);
    if (ray_callback.hasHit()) {
        btRigidBody* body = btRigidBody::upcast(ray_callback.m_collisionObject);
        if (body) {
            body->setActivationState(ACTIVE_TAG);
            btVector3 impulse = v1 - v0;
            impulse.normalize();
            float impulseStrength = 50.f;
            impulse *= impulseStrength;
            btVector3 relPos = ray_callback.m_hitPointWorld - body->getCenterOfMassPosition();
            body->applyImpulse(impulse,relPos);
        }

        drag_mode = DRAG_BALL;
    } else {
        drag_mode = DRAG_CAMERA;
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

    switch (drag_mode) {
    case DRAG_CAMERA:
        zenith  += diff.y() * 0.01;
        azimuth += diff.x() * 0.01;
        zenith = qBound(static_cast<double>(FLT_MIN), zenith, pi - FLT_MIN);
        updateGL();
        break;
    case DRAG_BALL:
        /// @todo INSERT MAGIC HERE
        break;
    }
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
