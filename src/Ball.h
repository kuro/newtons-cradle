
#pragma once

#include <QObject>

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

protected:
    Scene* scene;
    btHingeConstraint* hinge;
    btRigidBody* rigid_body;
};
