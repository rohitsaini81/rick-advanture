#include "skeleton_player.h"
#include "../../Controls/camera.h"
#include "../../ETC/global_var.h"
#include "raymath.h"
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <algorithm>

#include <ozz/base/span.h>
#include <ozz/base/maths/quaternion.h>
#include <ozz/geometry/runtime/skinning_job.h>

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



    anim.LoadAnimation("idle", (project_dir+"/assets/animrick/idle.ozz").c_str());
    anim.LoadAnimation("walk", (project_dir+"/assets/animrick/walk.ozz").c_str());
    anim.LoadAnimation("jump", (project_dir+"/assets/animrick/jump.ozz").c_str());

    anim.SetAnimation("idle");
    anim.SetLooping(true);
    anim.Play();



    // Setup animation (don't auto-play - we'll control it)
    anim.SetLooping(true);

    CreatePhysicsBody(startPos);

    LoadSkinnedModel(project_dir + "/assets/rick/character.glb");
    BuildBindPose();
    BuildBoneRemap();

    std::cout << "SkeletonPlayer constructed!" << std::endl;
}

SkeletonPlayer::~SkeletonPlayer()
{
    world->removeRigidBody(body);
    delete body;
    delete motionState;
    delete shape;

    if (model.meshCount > 0) {
        UnloadModel(model);
    }
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

    RunSkinningJob();
    SyncRotation();
}

void SkeletonPlayer::Render()
{
    Vector3 position = GetPosition();

    // Align mesh feet with ground (offset from capsule center)
    position.y -= capsuleHeight * 0.5f;

    // Use camera yaw only to orient mesh (ignore pitch).
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    forward.y = 0.0f;
    if (Vector3Length(forward) < 0.0001f) forward = {0.0f, 0.0f, 1.0f};
    forward = Vector3Normalize(forward);
    float yaw = atan2f(forward.x, forward.z);
    renderRotation = QuaternionFromEuler(0.0f, yaw, 0.0f);

    Vector3 axis = {0.0f, 1.0f, 0.0f};
    float angle = 0.0f;
    QuaternionToAxisAngle(renderRotation, &axis, &angle);
    DrawModelEx(model, position, axis, angle * RAD2DEG, {modelScale, modelScale, modelScale}, WHITE);
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

void SkeletonPlayer::LoadSkinnedModel(const std::string& modelPath)
{
    model = LoadModel(modelPath.c_str());
    if (model.meshCount <= 0) {
        std::cerr << "Failed to load model or no meshes: " << modelPath << std::endl;
        return;
    }

    mesh = &model.meshes[0];
    vertexCount = mesh->vertexCount;

    std::cout << "Mesh vertices: " << vertexCount << std::endl;
    std::cout << "Model boneCount: " << model.boneCount << std::endl;
    std::cout << "Mesh boneCount: " << mesh->boneCount << std::endl;
    std::cout << "Mesh boneIds: " << (mesh->boneIds ? "present" : "missing") << std::endl;
    std::cout << "Mesh boneWeights: " << (mesh->boneWeights ? "present" : "missing") << std::endl;

    restPositions.resize(vertexCount);
    restNormals.resize(vertexCount);
    skinnedPositions.resize(vertexCount * 3);
    skinnedNormals.resize(vertexCount * 3);

    // Copy rest positions and normals
    for (int i = 0; i < vertexCount; ++i) {
        restPositions[i] = ozz::math::Float3(
            mesh->vertices[i * 3 + 0],
            mesh->vertices[i * 3 + 1],
            mesh->vertices[i * 3 + 2]);

        if (mesh->normals) {
            restNormals[i] = ozz::math::Float3(
                mesh->normals[i * 3 + 0],
                mesh->normals[i * 3 + 1],
                mesh->normals[i * 3 + 2]);
        } else {
            restNormals[i] = ozz::math::Float3(0.0f, 1.0f, 0.0f);
        }
    }

    // Convert bone ids and weights (assume 4 influences per vertex)
    jointIndices.resize(vertexCount * 4);
    jointWeights.resize(vertexCount * 3);

    for (int i = 0; i < vertexCount; ++i) {
        const int base = i * 4;
        uint16_t id0 = mesh->boneIds ? mesh->boneIds[base + 0] : 0;
        uint16_t id1 = mesh->boneIds ? mesh->boneIds[base + 1] : 0;
        uint16_t id2 = mesh->boneIds ? mesh->boneIds[base + 2] : 0;
        uint16_t id3 = mesh->boneIds ? mesh->boneIds[base + 3] : 0;

        jointIndices[base + 0] = id0;
        jointIndices[base + 1] = id1;
        jointIndices[base + 2] = id2;
        jointIndices[base + 3] = id3;

        float w0 = mesh->boneWeights ? mesh->boneWeights[base + 0] : 1.0f;
        float w1 = mesh->boneWeights ? mesh->boneWeights[base + 1] : 0.0f;
        float w2 = mesh->boneWeights ? mesh->boneWeights[base + 2] : 0.0f;
        float w3 = mesh->boneWeights ? mesh->boneWeights[base + 3] : 0.0f;

        float sum = w0 + w1 + w2 + w3;
        if (sum > 0.0f) {
            w0 /= sum; w1 /= sum; w2 /= sum; w3 /= sum;
        } else {
            w0 = 1.0f; w1 = w2 = w3 = 0.0f;
        }

        // Store first 3 weights. The 4th is reconstructed in SkinningJob.
        jointWeights[i * 3 + 0] = w0;
        jointWeights[i * 3 + 1] = w1;
        jointWeights[i * 3 + 2] = w2;
    }

    if (model.boneCount <= 0 || model.bones == nullptr || model.bindPose == nullptr) {
        std::cerr << "Model has no bones/bind pose; skinning will not work." << std::endl;
    }
}

void SkeletonPlayer::BuildBindPose()
{
    if (model.boneCount <= 0 || model.bones == nullptr || model.bindPose == nullptr) return;

    std::vector<ozz::math::Float4x4> bindLocal(model.boneCount);
    std::vector<ozz::math::Float4x4> bindModel(model.boneCount);

    for (int i = 0; i < model.boneCount; ++i) {
        const Transform& t = model.bindPose[i];
        ozz::math::Float3 tr(t.translation.x, t.translation.y, t.translation.z);
        ozz::math::Float3 sc(t.scale.x, t.scale.y, t.scale.z);
        ozz::math::Quaternion rq(t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w);
        bindLocal[i] = ozz::math::Float4x4::FromAffine(tr, rq, sc);

        int parent = model.bones[i].parent;
        if (parent >= 0) {
            bindModel[i] = bindModel[parent] * bindLocal[i];
        } else {
            bindModel[i] = bindLocal[i];
        }
    }

    inverseBindPoses.resize(model.boneCount);
    skinningMatrices.resize(model.boneCount);

    for (int i = 0; i < model.boneCount; ++i) {
        inverseBindPoses[i] = ozz::math::Invert(bindModel[i]);
    }
}

void SkeletonPlayer::BuildBoneRemap()
{
    if (model.boneCount <= 0 || model.bones == nullptr) return;

    modelBoneToOzz.assign(model.boneCount, -1);
    auto normalize = [](std::string s) {
        // Strip mixamo prefixes
        const std::string mixamo = "mixamorig:";
        if (s.rfind(mixamo, 0) == 0) s = s.substr(mixamo.size());
        if (s.rfind("Armature", 0) == 0) s = "Root";

        // Common suffix cleanup
        const std::string end = "_End";
        if (s.size() > end.size() && s.compare(s.size() - end.size(), end.size(), end) == 0) {
            s = s.substr(0, s.size() - end.size());
        }

        // Normalize separators
        std::replace(s.begin(), s.end(), '_', '.');
        return s;
    };

    auto remapMixamoToModel = [](const std::string& s) {
        // Map common Mixamo names to this model's naming style
        if (s == "Hips") return std::string("Hip");
        if (s == "Spine1") return std::string("Chest");
        if (s == "LeftShoulder") return std::string("L.Collar");
        if (s == "RightShoulder") return std::string("R.Collar");
        if (s == "LeftArm") return std::string("L.Bicep");
        if (s == "RightArm") return std::string("R.Bicep");
        if (s == "LeftForeArm") return std::string("L.Forearm");
        if (s == "RightForeArm") return std::string("R.Forearm");
        if (s == "LeftHand") return std::string("L.Hand");
        if (s == "RightHand") return std::string("R.Hand");
        if (s == "LeftUpLeg") return std::string("L.Thigh");
        if (s == "RightUpLeg") return std::string("R.Thigh");
        if (s == "LeftLeg") return std::string("L.Calf");
        if (s == "RightLeg") return std::string("R.Calf");
        if (s == "LeftFoot") return std::string("L.Foot");
        if (s == "RightFoot") return std::string("R.Foot");
        if (s == "LeftToeBase") return std::string("L.Toe");
        if (s == "RightToeBase") return std::string("R.Toe");
        return s;
    };

    std::unordered_map<std::string, int> jointIndex;
    auto names = anim.GetSkeleton().joint_names();
    std::cout << "Ozz joints: " << anim.GetSkeleton().num_joints() << std::endl;
    for (int i = 0; i < anim.GetSkeleton().num_joints(); ++i) {
        std::string n = remapMixamoToModel(normalize(names[i]));
        jointIndex[n] = i;
    }

    int matched = 0;
    for (int i = 0; i < model.boneCount; ++i) {
        std::string modelName = normalize(model.bones[i].name);
        auto it = jointIndex.find(modelName);
        if (it != jointIndex.end()) {
            modelBoneToOzz[i] = it->second;
            matched++;
        }
    }

    std::cout << "Ozz bone mapping: " << matched << "/" << model.boneCount << " bones matched" << std::endl;
    std::cout << "First 10 model bones:" << std::endl;
    for (int i = 0; i < model.boneCount && i < 10; ++i) {
        std::cout << "  " << i << ": " << model.bones[i].name << std::endl;
    }
    std::cout << "First 10 ozz joints:" << std::endl;
    for (int i = 0; i < anim.GetSkeleton().num_joints() && i < 10; ++i) {
        std::cout << "  " << i << ": " << names[i] << std::endl;
    }
}

void SkeletonPlayer::RunSkinningJob()
{
    if (!mesh || model.boneCount <= 0 || anim.GetModelMatrices().empty()) return;

    const auto& ozzModels = anim.GetModelMatrices();

    for (int i = 0; i < model.boneCount; ++i) {
        int ozzIndex = (i < (int)modelBoneToOzz.size()) ? modelBoneToOzz[i] : -1;
        if (ozzIndex >= 0 && ozzIndex < (int)ozzModels.size()) {
            skinningMatrices[i] = ozzModels[ozzIndex] * inverseBindPoses[i];
        } else {
            skinningMatrices[i] = ozz::math::Float4x4::identity();
        }
    }

    ozz::geometry::SkinningJob job;
    job.vertex_count = vertexCount;
    job.influences_count = 4;
    job.joint_matrices = ozz::span<const ozz::math::Float4x4>(skinningMatrices.data(),
                                                              skinningMatrices.data() + skinningMatrices.size());
    job.joint_indices = ozz::span<const uint16_t>(jointIndices.data(),
                                                  jointIndices.data() + jointIndices.size());
    job.joint_indices_stride = sizeof(uint16_t) * 4;

    job.joint_weights = ozz::span<const float>(jointWeights.data(),
                                               jointWeights.data() + jointWeights.size());
    job.joint_weights_stride = sizeof(float) * 3;

    const float* inPos = &restPositions[0].x;
    const float* inNrm = &restNormals[0].x;
    job.in_positions = ozz::span<const float>(inPos, inPos + vertexCount * 3);
    job.in_positions_stride = sizeof(float) * 3;
    job.in_normals = ozz::span<const float>(inNrm, inNrm + vertexCount * 3);
    job.in_normals_stride = sizeof(float) * 3;

    job.out_positions = ozz::span<float>(skinnedPositions.data(),
                                         skinnedPositions.data() + skinnedPositions.size());
    job.out_positions_stride = sizeof(float) * 3;
    job.out_normals = ozz::span<float>(skinnedNormals.data(),
                                       skinnedNormals.data() + skinnedNormals.size());
    job.out_normals_stride = sizeof(float) * 3;

    if (!job.Run()) {
        std::cerr << "SkinningJob failed" << std::endl;
        return;
    }

    UpdateSkinnedMeshBuffers();
}

void SkeletonPlayer::UpdateSkinnedMeshBuffers()
{
    if (!mesh) return;

    std::memcpy(mesh->vertices, skinnedPositions.data(), skinnedPositions.size() * sizeof(float));
    if (mesh->normals) {
        std::memcpy(mesh->normals, skinnedNormals.data(), skinnedNormals.size() * sizeof(float));
    }

    UpdateMeshBuffer(*mesh, 0, mesh->vertices, mesh->vertexCount * 3 * sizeof(float), 0);
    if (mesh->normals) {
        UpdateMeshBuffer(*mesh, 2, mesh->normals, mesh->vertexCount * 3 * sizeof(float), 0);
    }
}
