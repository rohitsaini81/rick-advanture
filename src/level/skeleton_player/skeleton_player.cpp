#include "skeleton_player.h"
#include "../../Controls/camera.h"
#include "../../ETC/global_var.h"
#include "raymath.h"
#include <iostream>

SkeletonPlayer::SkeletonPlayer(btDiscreteDynamicsWorld* world, const std::string& skeletonPath,
                               const std::string& animationPath, const Vector3& startPos)
    : world(world), animationTime(0.0f), wasMoving(false)
{
    // Load skeleton and animation
    if (!anim.LoadSkeleton(skeletonPath.c_str())) {
        std::cerr << "Failed to load skeleton: " << skeletonPath << std::endl;
        return;
    }
    if (!anim.LoadAnimation(animationPath.c_str())) {
        std::cerr << "Failed to load animation: " << animationPath << std::endl;
        return;
    }

    // Setup animation (don't auto-play - we'll control it)
    anim.SetLooping(true);
    anim.Stop();
    anim.Reset();

    CreatePhysicsBody(startPos);

    std::cout << "SkeletonPlayer constructed!" << std::endl;
}

SkeletonPlayer::~SkeletonPlayer()
{
    world->removeRigidBody(body);
    delete body;
    delete motionState;
    delete shape;
}

void SkeletonPlayer::CreatePhysicsBody(const Vector3& startPos)
{
    float radius = 0.45f;
    capsuleHeight = 1.8f;  // Approximate skeleton height

    shape = new btCapsuleShape(radius, capsuleHeight);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));

    motionState = new btDefaultMotionState(startTransform);

    float mass = 1.0f;
    btVector3 inertia(0, 0, 0);
    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    body = new btRigidBody(rbInfo);

    body->setAngularFactor(btVector3(0, 1, 0));
    body->setActivationState(DISABLE_DEACTIVATION);

    world->addRigidBody(body);
}

Vector3 SkeletonPlayer::GetPosition() const
{
    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    btVector3 p = trans.getOrigin();

    return { p.getX(), p.getY(), p.getZ() };
}

void SkeletonPlayer::SyncRotation()
{
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

    float yaw = atan2f(forward.x, forward.z);
    btQuaternion q;
    q.setEuler(0, -yaw, 0);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    trans.setRotation(q);
    body->getMotionState()->setWorldTransform(trans);
}

void SkeletonPlayer::UpdateAnimation(float deltaTime, bool isMoving)
{
    if (isMoving) {
        // Play animation when moving
        if (!anim.IsPlaying()) {
            anim.Play();
        }
        anim.Update(deltaTime);
    } else {
        // Stop animation when not moving
        if (anim.IsPlaying()) {
            anim.Stop();
            // Keep it at first frame (time 0)
            anim.Reset();
        }
    }
}

void SkeletonPlayer::Update(float deltaTime)
{
    if (!body) return;

    float playerMoveSpeed = 5.0f;

    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

    forward.y = 0;
    right.y = 0;

    Vector3 move = {0, 0, 0};

    if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
    if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);
    if (IsKeyPressed(KEY_SPACE)) { body->applyCentralImpulse(btVector3(0, 5, 0)); }

    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
        playerMoveSpeed = 50;
    } else {
        playerMoveSpeed = 10;
    }

    bool isMoving = Vector3Length(move) > 0.01f;
    move = Vector3Normalize(move);

    btVector3 vel(move.x * playerMoveSpeed, body->getLinearVelocity().y(), move.z * playerMoveSpeed);
    body->setLinearVelocity(vel);

    // Update animation based on movement state
    UpdateAnimation(deltaTime, isMoving);

    SyncRotation();
}

void SkeletonPlayer::Render()
{
    // Draw skeleton at physics body position
    Vector3 position = GetPosition();

    // Align skeleton feet with ground (offset from capsule center)
    position.y -= capsuleHeight * 0.5f;

    renderer.DrawSkeletonTransformed(
        anim.GetSkeleton(),
        anim.GetModelMatrices(),
        position,
        QuaternionIdentity(),
        1.0f,
        BLUE,   // joint color
        RED     // bone color
    );
}
