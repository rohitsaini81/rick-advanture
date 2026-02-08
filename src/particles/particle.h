#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <raylib.h>

// ----------------------------

struct Particle {
    btRigidBody* body;
    float radius;
};

// ----------------------------

class particle {
public:

    // Constructor: pass existing Bullet world
    particle(btDiscreteDynamicsWorld* world);

    // Create particles
    void initParticles(
        int count,
        float radius,
        float mass,
        Vector3 minPos,
        Vector3 maxPos
    );

    // Update physics (optional if handled outside)
    void update(float dt);

    // Draw all particles
    void draw();

    // Cleanup
    void destroy();

private:

    btDiscreteDynamicsWorld* mWorld;

    std::vector<Particle> mParticles;
};

#endif // PARTICLE_H
