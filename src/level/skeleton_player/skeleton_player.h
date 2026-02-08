#ifndef SKELETON_PLAYER_H
#define SKELETON_PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include <btBulletDynamicsCommon.h>
#include "../../ozz/skeleton_animation.h"
#include "../../ozz/skeleton_renderer.h"


enum class PlayerAnimState;

class SkeletonPlayer
{
public:
    SkeletonPlayer(btDiscreteDynamicsWorld* world, const std::string& skeletonPath,
                   const std::string& animationPath, const Vector3& startPos);
    ~SkeletonPlayer();
void SetAnimationState(PlayerAnimState newState);
    void Update(float deltaTime);
    void Render();
    float skeletonScale = 0.02f; // default 1
    Vector3 GetPosition() const;
    btRigidBody* GetBody() const { return body; }

private:
    void CreatePhysicsBody(const Vector3& startPos);
    void SyncRotation();
    void UpdateAnimation(float deltaTime, bool isMoving);

private:
    btDiscreteDynamicsWorld* world;

    btRigidBody* body;
    btCollisionShape* shape;
    btDefaultMotionState* motionState;

    SkeletonAnimation anim;
    SkeletonRenderer renderer;

    float animationTime;
    bool wasMoving;
    float capsuleHeight;
    Quaternion renderRotation;
};






enum class PlayerAnimState {
    Idle,
    Walk,
    Jump
};

extern PlayerAnimState animState;

#endif
