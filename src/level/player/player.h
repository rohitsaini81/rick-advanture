#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include <btBulletDynamicsCommon.h>
#include <array>
#include <string>
#include <vector>

#include "../../ozz/skeleton_animation.h"

class Player
{
public:
    Player(btDiscreteDynamicsWorld* world, const std::string& modelPath, const Vector3& startPos);
    ~Player();

    void Update(float deltaTime);
    void Render();

    Vector3 GetPosition() const;
    btRigidBody* GetBody() const { return body; }

private:
    void CreatePhysicsBody(const Vector3& startPos);
    void SyncRotation();
    void CalculateModelHeight();
    void InitOzzAnimation();
    void UpdateOzzAnimation(float deltaTime, bool isMoving, bool jumpPressed);
    void ApplyOzzToModel();
    void SetOzzAnimation(const std::string& name, bool looping);

private:
    btDiscreteDynamicsWorld* world;

    btRigidBody* body;
    btCollisionShape* shape;
    btDefaultMotionState* motionState;

    Model model;

    float modelHeight = 0.0f;

    // Ozz-driven animation for skinned mesh
    SkeletonAnimation ozzAnim;
    ModelAnimation ozzModelAnim{};
    std::vector<Transform> ozzFramePose;
    std::array<Transform*, 1> ozzFramePosePtrs{};
    std::vector<int> modelBoneToOzz;
    std::string currentOzzAnim;
    bool ozzReady = false;
};


extern Player* player;

#endif
