
#pragma once

#include <QObject>
#include <Cg/cg.h>

class Scene;

class btCollisionShape;
class btRigidBody;
class btHingeConstraint;

class Ball
{
public:
    static void init (Scene* scene);
    static void deinit ();

public:
    Ball (Scene* scene, int index, int ball_count);
    ~Ball ();

    void draw ();

public:
    static qreal radius;
protected:
    static btCollisionShape* shape;
    static quint32 dlist;

    static struct cg
    {
        CGprogram program;
        CGparameter eye;
    } cg;


protected:
    Scene* scene;
    btHingeConstraint* hinge;
    btRigidBody* rigid_body;

    friend class Scene;
};
