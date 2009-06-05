
#include "Ball.h"
#include "Scene.h"

#include <btBulletDynamicsCommon.h>

#include <QtDebug>

btCollisionShape* Ball::shape = NULL;
qreal Ball::radius = 1;
quint32 Ball::dlist = 0;

void Ball::init ()
{
    shape = new btSphereShape(1);
    Q_ASSERT(shape);

    dlist = glGenLists(1);
    Q_ASSERT(glIsList(dlist));
    GLUquadric* quadric = gluNewQuadric();
    glNewList(dlist, GL_COMPILE);
    gluSphere(quadric, radius, 16, 16);
    glEndList();
    gluDeleteQuadric(quadric);
}

void Ball::deinit ()
{
    delete shape;
}

Ball::Ball (Scene* scene, int index, int ball_count) :
    scene(scene),
    rigid_body(NULL)
{
    btScalar mass = 1;
    btVector3 inertia(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    qreal x = index * radius * 2;
    x += radius - (radius * ball_count);

    btDefaultMotionState* motion_state =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                 btVector3(x, 0, 0)));

    btRigidBody::btRigidBodyConstructionInfo con_info
        (mass, motion_state, shape, inertia);

    con_info.m_restitution = 0.9;
    con_info.m_linearSleepingThreshold = 0.0;

    rigid_body = new btRigidBody(con_info);

    scene->world->addRigidBody(rigid_body);

    hinge = new btHingeConstraint (*rigid_body,
                                   btVector3(0, 10, 0),
                                   btVector3(0, 0, 1));
    scene->world->addConstraint(hinge, true);
}

Ball::~Ball ()
{
    scene->world->removeConstraint(hinge);
    scene->world->removeRigidBody(rigid_body);

    delete hinge;
    delete rigid_body->getMotionState();
    delete rigid_body;
}

void Ball::draw ()
{
    float mat[16];
    btTransform transform;

    rigid_body->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(mat);

    glColor3f(0, 0, 1);

    glPushMatrix();
    glMultMatrixf(mat);
    glCallList(dlist);
    glPopMatrix();
}
