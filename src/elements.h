// elements.h
#pragma once

#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <btBulletDynamicsCommon.h>

enum class ElementType {
    BOX,
    CUBE
};


extern btVector3 inertia;





class Elements {
public:
    btCollisionShape* shape = nullptr;
    btDefaultMotionState* motion = nullptr;
    btRigidBody* body = nullptr;
    float mass = 1.0f;
    std::string object_name;

    Elements(ElementType type, btDiscreteDynamicsWorld* dynamicsWorld);
    void destroy();
};
