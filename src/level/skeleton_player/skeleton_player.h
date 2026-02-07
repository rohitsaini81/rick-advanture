#ifndef SKELETON_PLAYER_H
#define SKELETON_PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include <btBulletDynamicsCommon.h>
#include "../../ozz/skeleton_animation.h"
#include "../../ozz/skeleton_renderer.h"

class SkeletonPlayer
{
public:
    SkeletonPlayer(btDiscreteDynamicsWorld* world, const std::string& skeletonPath,
                   const std::string& animationPath, const Vector3& startPos);
    ~SkeletonPlayer();

    void Update(float deltaTime);
    void Render();

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
};

#endif
