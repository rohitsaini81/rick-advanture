// objects.h
// #pragma once
#include <btBulletDynamicsCommon.h>
#include <iostream>
#include "physics.h"

enum class ElementType {
    BOX,
    CUBE
};

class Elements {
public:
    btCollisionShape* shape = nullptr;
    btDefaultMotionState* motion = nullptr;
    btRigidBody* body = nullptr;
    float mass = 1.0f;
    

    Elements(ElementType type, btDiscreteDynamicsWorld* dynamicsWorld) {
        std::cout<<"---------------------------------------";
    
        btVector3 inertia = btVector3(0, 0, 0);

        if (type == ElementType::BOX) {
            std::cout << "Creating BOX" << std::endl;
            shape = new btBoxShape(btVector3(1, 1, 1));
            mass = 200.0f;
        } else if (type == ElementType::CUBE) {
            // std::cout << "Creating CUBE" << std::endl;
            // shape = new btBoxShape(btVector3(2, 2, 2)); // cube with different size
            // mass = 4.0f;
        }

        motion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
        shape->calculateLocalInertia(mass, inertia);

        btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
        body = new btRigidBody(info);
    }

    void destroy() {
        delete body;
        delete motion;
        delete shape;
    }
};



void CREATE_ELEM(){
    try{
        if(dynamicsWorld){
            std::cout<<"fine working ?? "<<std::endl;
    Elements box(ElementType::BOX, dynamicsWorld);
    std::cout<<"Note------------------------"<<box.mass;
    std::cout << "dynamicsWorld ptr: " << dynamicsWorld << std::endl;
    std::cout << "box.body ptr: " << box.body << std::endl;
    
    if (dynamicsWorld && box.body) {
        std::cout << "About to add rigid body\n";
        dynamicsWorld->addRigidBody(box.body);  // crash happens here?
        std::cout << "Rigid body added\n";
    } else {
        std::cerr << "Invalid world or body\n";
    }
    

        }
    }
 catch (const std::exception& e) {
    std::cerr << "Caught std::exception: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "Caught unknown exception" << std::endl;
}
}
