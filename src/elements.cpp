// elements.cpp
#include "elements.h"
#include "iostream"
Elements::Elements(ElementType type, btDiscreteDynamicsWorld* dynamicsWorld) {
    std::cout << "---------------------------------------\n";
    object_name = "testing 1";
    btVector3 inertia = btVector3(0, 0, 0);

    if (type == ElementType::BOX) {
        std::cout << "Creating BOX" << std::endl;
        shape = new btBoxShape(btVector3(1, 1, 1));
        mass = 50.0f;
    } else if (type == ElementType::CUBE) {
        // You can customize later
    }

    motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
    body = new btRigidBody(info);
}

void Elements::destroy() {
    delete body;
    delete motion;
    delete shape;
}
