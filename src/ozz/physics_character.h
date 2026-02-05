#ifndef PHYSICS_CHARACTER_H
#define PHYSICS_CHARACTER_H

#include "raylib.h"
#include <ozz/base/maths/simd_math.h>
#include <ozz/animation/runtime/skeleton.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
// #include "physics.h"

class PhysicsCharacter {
public:
    PhysicsCharacter(btDynamicsWorld* world);
    ~PhysicsCharacter();
    void ApplyForce(const btVector3& dir, float strength);

    // Initialize character with skeleton - creates capsule shapes for each joint
    void InitializeFromSkeleton(const ozz::animation::Skeleton& skeleton);

    // Update physics bodies from skeleton model matrices
    void UpdateFromSkeleton(const std::vector<ozz::math::Float4x4>& model_matrices);

    // Apply external forces to the character
    // void ApplyForce(const Vector3& force_direction, float magnitude = 1.0f);
    void ApplyImpulse(const Vector3& direction, float magnitude = 1.0f);

    // Get physics bodies for the skeleton joints
    const std::vector<btRigidBody*>& GetBodies() const { return rigid_bodies_; }

    // Set physics enabled/disabled
    void SetEnabled(bool enabled);
    bool IsEnabled() const { return is_enabled_; }

    // Update visualization - no simulation, just visual updates
    void Update(float dt);

    btDynamicsWorld* world_;
    std::vector<btRigidBody*> rigid_bodies_;
    std::vector<btCollisionShape*> collision_shapes_;
    bool is_enabled_;
};

#endif // PHYSICS_CHARACTER_H
