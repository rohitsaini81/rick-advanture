#include "physics_character.h"
#include <ozz/base/maths/simd_math.h>
#include <cmath>

PhysicsCharacter::PhysicsCharacter(btDynamicsWorld* world)
    : world_(world), is_enabled_(true) {
}

PhysicsCharacter::~PhysicsCharacter() {
    for (auto body : rigid_bodies_) {
        if (world_) {
            world_->removeRigidBody(body);
        }
        delete body->getMotionState();
        delete body;
    }
    for (auto shape : collision_shapes_) {
        delete shape;
    }
}

void PhysicsCharacter::ApplyForce(const btVector3& dir, float strength)
{
    PhysicsCharacter::rigid_bodies_[0]->applyCentralForce(dir * strength);
}



void PhysicsCharacter::InitializeFromSkeleton(const ozz::animation::Skeleton& skeleton) {
    // Create capsule shapes for each joint
    for (int i = 0; i < skeleton.num_joints(); i++) {
        // Create a small capsule for each joint
        btCapsuleShape* capsule = new btCapsuleShape(0.05f, 0.2f);
        collision_shapes_.push_back(capsule);

        // Create motion state
        btDefaultMotionState* motion_state = new btDefaultMotionState();

        // Create rigid body
        btRigidBody::btRigidBodyConstructionInfo rigid_body_info(
            0.1f, motion_state, capsule, btVector3(0, 0, 0));

        rigid_body_info.m_restitution = 0.5f;
        rigid_body_info.m_friction = 0.7f;

        btRigidBody* rigid_body = new btRigidBody(rigid_body_info);
        rigid_body->setCollisionFlags(
            rigid_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

        if (world_) {
            world_->addRigidBody(rigid_body);
        }

        rigid_bodies_.push_back(rigid_body);
    }
}

void PhysicsCharacter::UpdateFromSkeleton(
    const std::vector<ozz::math::Float4x4>& model_matrices) {
    if (!is_enabled_) {
        return;
    }

    for (size_t i = 0; i < rigid_bodies_.size() && i < model_matrices.size(); i++) {
        const auto& matrix = model_matrices[i];

        // Extract position using SIMD accessors
        btVector3 position(
            ozz::math::GetX(matrix.cols[3]),
            ozz::math::GetY(matrix.cols[3]),
            ozz::math::GetZ(matrix.cols[3]));

        // Extract rotation (from the 3x3 part of the matrix)
        btMatrix3x3 rotation(
            ozz::math::GetX(matrix.cols[0]),
            ozz::math::GetX(matrix.cols[1]),
            ozz::math::GetX(matrix.cols[2]),
            ozz::math::GetY(matrix.cols[0]),
            ozz::math::GetY(matrix.cols[1]),
            ozz::math::GetY(matrix.cols[2]),
            ozz::math::GetZ(matrix.cols[0]),
            ozz::math::GetZ(matrix.cols[1]),
            ozz::math::GetZ(matrix.cols[2]));

        btQuaternion quat;
        rotation.getRotation(quat);

        // Update rigid body transform
        btTransform transform;
        transform.setOrigin(position);
        transform.setRotation(quat);

        rigid_bodies_[i]->getMotionState()->setWorldTransform(transform);
        rigid_bodies_[i]->setWorldTransform(transform);
    }
}

// void PhysicsCharacter::ApplyForce(const Vector3& force_direction, float magnitude) {
//     if (!is_enabled_ || rigid_bodies_.empty()) {
//         return;
//     }
//
//     // Apply force to root body (first joint)
//     btVector3 force(force_direction.x * magnitude,
//                     force_direction.y * magnitude,
//                     force_direction.z * magnitude);
//
//     rigid_bodies_[0]->applyCentralForce(force);
// }

void PhysicsCharacter::ApplyImpulse(const Vector3& direction, float magnitude) {
    if (!is_enabled_ || rigid_bodies_.empty()) {
        return;
    }

    btVector3 impulse(direction.x * magnitude,
                      direction.y * magnitude,
                      direction.z * magnitude);

    rigid_bodies_[0]->applyCentralImpulse(impulse);
}

void PhysicsCharacter::SetEnabled(bool enabled) {
    is_enabled_ = enabled;
}

void PhysicsCharacter::Update(float dt) {
    // Physics simulation is handled by the world
    // This is just for any character-specific updates
}
