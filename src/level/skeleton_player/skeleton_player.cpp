#include "skeleton_player.h"
#include "../../Controls/camera.h"
#include "../../ETC/global_var.h"
#include "raymath.h"
#include <iostream>

PlayerAnimState animState = PlayerAnimState::Idle;

SkeletonPlayer::SkeletonPlayer(btDiscreteDynamicsWorld* world, const std::string& skeletonPath,
                               const std::string& animationPath, const Vector3& startPos)
    : world(world), animationTime(0.0f), wasMoving(false), renderRotation(QuaternionIdentity())
{
    // Load skeleton and animation
    if (!anim.LoadSkeleton(skeletonPath.c_str())) {
        std::cerr << "Failed to load skeleton: " << skeletonPath << std::endl;
        return;
    }
    // if (!anim.LoadAnimation(animationPath.c_str())) {
    //     std::cerr << "Failed to load animation: " << animationPath << std::endl;
    //     return;
    // }



    if (!anim.LoadSkeleton(skeletonPath.c_str())) {
        std::cerr << "Failed to load skeleton\n";
        return;
    }

    anim.LoadAnimation("idle", (project_dir+"/assets/animrick/idle.ozz").c_str());
    anim.LoadAnimation("walk", (project_dir+"/assets/animrick/walk.ozz").c_str());
    anim.LoadAnimation("jump", (project_dir+"/assets/animrick/jump.ozz").c_str());

    anim.SetAnimation("idle");
    anim.SetLooping(true);
    anim.Play();



    // Setup animation (don't auto-play - we'll control it)
    anim.SetLooping(true);
    // anim.Stop();
    // anim.Reset();

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
    renderRotation = QuaternionFromEuler(0.0f, -yaw, 0.0f);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    trans.setRotation(q);
    body->getMotionState()->setWorldTransform(trans);
}

void SkeletonPlayer::UpdateAnimation(float deltaTime, bool isMoving)
{
    if (!isMoving) {
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
    if (IsKeyPressed(KEY_SPACE))
    {
        body->applyCentralImpulse(btVector3(0, 5, 0));
        SetAnimationState(PlayerAnimState::Jump);
    }

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
    // UpdateAnimation(deltaTime, isMoving);

    if (animState != PlayerAnimState::Jump) {
        if (isMoving)
            SetAnimationState(PlayerAnimState::Walk);
        else
            SetAnimationState(PlayerAnimState::Idle);
    }

    anim.Update(deltaTime);
    if (animState == PlayerAnimState::Jump && !anim.IsPlaying()) {
        SetAnimationState(isMoving ? PlayerAnimState::Walk : PlayerAnimState::Idle);
    }

    SyncRotation();
}

void SkeletonPlayer::Render()
{
    // Draw skeleton at physics body position
    Vector3 position = GetPosition();

    // Align skeleton feet with ground (offset from capsule center)
    position.y -= capsuleHeight * 0.5f;

    // Use current camera direction for model orientation (avoids 1-frame lag if camera updates after player update)
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    float yaw = atan2f(forward.x, forward.z);
    // Model faces -Z; add 180 deg so its back faces the camera direction.
    renderRotation = QuaternionFromEuler(0.0f, -yaw + PI, 0.0f);

    renderer.DrawSkeletonTransformed(
        anim.GetSkeleton(),
        anim.GetModelMatrices(),
        position,
        renderRotation,
        skeletonScale,
        BLUE,   // joint color
        RED     // bone color
    );
}










void SkeletonPlayer::SetAnimationState(PlayerAnimState newState)
{
    if (newState == animState) return;

    animState = newState;

    switch (animState) {
    case PlayerAnimState::Idle:
        anim.SetAnimation("idle");
        anim.SetLooping(true);
        anim.Play();
        break;

    case PlayerAnimState::Walk:
        anim.SetAnimation("walk");
        anim.SetLooping(true);
        anim.Play();
        break;

    case PlayerAnimState::Jump:
        anim.SetAnimation("jump");
        anim.SetLooping(false);
        anim.Play();
        break;
    }
}
