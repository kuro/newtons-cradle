
#include "Scene.moc"

#include <QMouseEvent>

#include <math.h>
#include <float.h>

#include <QtDebug>
#include <QTimer>

const qreal pi = 4.0 * atan(1.0);

#define ZNEAR 0.1
#define ZFAR 100.0

Scene::Scene () :
    distance(10.0),
    zenith(pi/2.0),
    azimuth(-pi/2.0)
{
    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), SLOT(updateGL()));
    //timer->start(0.01);
}

Scene::~Scene ()
{
}

void Scene::initializeGL ()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
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

    qglColor("red");
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 0.5f);
    glVertex3f( 1.0f, -1.0f, 0.5f);
    glVertex3f( 1.0f,  1.0f, 0.5f);
    glVertex3f(-1.0f,  1.0f, 0.5f);
    glEnd();

    qglColor("blue");
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -0.5f);
    glVertex3f( 1.0f, -1.0f, -0.5f);
    glVertex3f( 1.0f,  1.0f, -0.5f);
    glVertex3f(-1.0f,  1.0f, -0.5f);
    glEnd();

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
