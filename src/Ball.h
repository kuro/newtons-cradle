
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
    static void init ();
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
        CGcontext context;
        CGprofile vertex_profile;
        CGprofile fragment_profile;
        CGprogram program;
        CGparameter eye;
    } cg;


protected:
    Scene* scene;
    btHingeConstraint* hinge;
    btRigidBody* rigid_body;

    friend class Scene;
};
