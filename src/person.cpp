#include "person.h"
#include <iostream>

// Bullet mass for dynamic body
static const float PERSON_MASS = 1.0f;

Person::Person(Vector3 pos, PersonType t) : position(pos), type(t) {
    InitPhysics(pos);
}

Person::~Person() {
    CleanupPhysics();
}

void Person::InitPhysics(Vector3 pos) {
    // Create a simple capsule shape or box shape for the character
    shape = new btCapsuleShape(0.3f, 1.2f); // radius, height

    // Set Bullet transform
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

    motionState = new btDefaultMotionState(startTransform);

    btVector3 inertia(0, 0, 0);
    shape->calculateLocalInertia(PERSON_MASS, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(PERSON_MASS, motionState, shape, inertia);
    body = new btRigidBody(rbInfo);

    // Optional: make the body not rotate (like a character controller)
    body->setAngularFactor(0);
    

    // Note: You must add `body` to the Bullet dynamics world externally!
}

void Person::CleanupPhysics() {
    if (body) {
        delete body;
        body = nullptr;
    }

    if (motionState) {
        delete motionState;
        motionState = nullptr;
    }

    if (shape) {
        delete shape;
        shape = nullptr;
    }
}

void Person::Update(float deltaTime) {
    // Sync position from physics body
    if (body) {
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);
        btVector3 btPos = trans.getOrigin();
        position = { (float)btPos.getX(), (float)btPos.getY(), (float)btPos.getZ() };
    }

    // AI or behavior logic could go here
}

void Person::Render() const {
    Color color;

    switch (type) {
        case PersonType::ENEMY: color = RED; break;
        case PersonType::ALLY: color = GREEN; break;
        case PersonType::NEUTRAL: color = BLUE; break;
    }

    // Draw as a capsule or cylinder (fake capsule with two spheres)
    DrawCylinder(position, 0.3f, 0.3f, 1.2f, 12, color);
        DrawSphere({ position.x, position.y + 0.6f, position.z }, 0.3f, color);
    DrawSphere({ position.x, position.y - 0.6f, position.z }, 0.3f, color);
}

Vector3 Person::GetPosition() const {
    return position;
}

void Person::SetPosition(Vector3 pos) {
    position = pos;
    if (body) {
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
        body->setWorldTransform(trans);
        body->getMotionState()->setWorldTransform(trans);
    }
}

btRigidBody* Person::GetRigidBody() {
    return body;
}

PersonType Person::GetType() const {
    return type;
}

std::string Person::GetTypeName() const {
    switch (type) {
        case PersonType::ENEMY: return "Enemy";
        case PersonType::ALLY: return "Ally";
        case PersonType::NEUTRAL: return "Neutral";
        default: return "Unknown";
    }
}
