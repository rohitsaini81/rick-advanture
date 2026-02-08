#include "particle.h"

// ----------------------------

particle::particle(btDiscreteDynamicsWorld* world)
{
    mWorld = world;
}

// ----------------------------

void particle::initParticles(
    int count,
    float radius,
    float mass,
    Vector3 minPos,
    Vector3 maxPos
) {
    for (int i = 0; i < count; i++) {

        // Random position
        float x = GetRandomValue(minPos.x, maxPos.x);
        float y = GetRandomValue(minPos.y, maxPos.y);
        float z = GetRandomValue(minPos.z, maxPos.z);

        btVector3 pos(x, y, z);

        // Collision shape
        auto* shape = new btSphereShape(radius);

        // Transform
        btTransform t;
        t.setIdentity();
        t.setOrigin(pos);

        // Inertia
        btVector3 inertia(0,0,0);
        shape->calculateLocalInertia(mass, inertia);

        // Motion state
        auto* motion =
            new btDefaultMotionState(t);

        // Body info
        btRigidBody::btRigidBodyConstructionInfo info(
            mass,
            motion,
            shape,
            inertia
        );

        // Rigid body
        auto* body = new btRigidBody(info);

        // Optional tuning
        body->setRestitution(0.6f);   // bounce
        body->setFriction(0.4f);
        body->setDamping(0.1f, 0.8f);

        // Add to world
        mWorld->addRigidBody(body);

        // Store
        mParticles.push_back({
            body,
            radius
        });
    }
}

// ----------------------------

void particle::update(float dt)
{
    // Only if you want this class
    // to step physics itself
    if (mWorld) {
        mWorld->stepSimulation(dt, 10);
    }
}

// ----------------------------

void particle::draw()
{
    for (auto& p : mParticles) {

        btTransform t;
        p.body->getMotionState()
               ->getWorldTransform(t);

        btVector3 pos = t.getOrigin();

        DrawSphere(
            {
                pos.x(),
                pos.y(),
                pos.z()
            },
            p.radius,
            ORANGE
        );
    }
}

// ----------------------------

void particle::destroy()
{
    for (auto& p : mParticles) {

        if (!p.body) continue;

        mWorld->removeRigidBody(p.body);

        delete p.body->getMotionState();
        delete p.body->getCollisionShape();
        delete p.body;

        p.body = nullptr;
    }

    mParticles.clear();
}
