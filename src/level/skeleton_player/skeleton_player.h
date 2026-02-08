#ifndef SKELETON_PLAYER_H
#define SKELETON_PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include <btBulletDynamicsCommon.h>
#include <array>
#include <string>
#include <vector>
#include "../../ozz/skeleton_animation.h"
#include <ozz/base/maths/vec_float.h>
#include <ozz/base/maths/simd_math.h>


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
    Vector3 GetPosition() const;
    btRigidBody* GetBody() const { return body; }

private:
    void CreatePhysicsBody(const Vector3& startPos);
    void SyncRotation();
    void LoadSkinnedModel(const std::string& modelPath);
    void BuildBindPose();
    void BuildBoneRemap();
    void RunSkinningJob();
    void UpdateSkinnedMeshBuffers();

private:
    btDiscreteDynamicsWorld* world;

    btRigidBody* body;
    btCollisionShape* shape;
    btDefaultMotionState* motionState;

    SkeletonAnimation anim;

    float animationTime;
    bool wasMoving;
    float capsuleHeight;
    Quaternion renderRotation;
    float modelScale = 0.02f;

    // Skinned mesh data
    Model model{};
    Mesh* mesh = nullptr;
    int vertexCount = 0;

    std::vector<ozz::math::Float3> restPositions;
    std::vector<ozz::math::Float3> restNormals;
    std::vector<float> skinnedPositions;
    std::vector<float> skinnedNormals;
    std::vector<uint16_t> jointIndices;
    std::vector<float> jointWeights;

    std::vector<int> modelBoneToOzz;
    std::vector<ozz::math::Float4x4> inverseBindPoses;
    std::vector<ozz::math::Float4x4> skinningMatrices;
};






enum class PlayerAnimState {
    Idle,
    Walk,
    Jump
};

extern PlayerAnimState animState;

#endif
