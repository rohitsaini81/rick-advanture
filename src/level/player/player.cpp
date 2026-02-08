#include "player.h"
#include "../../Controls/camera.h"
#include "../../ETC/global_var.h"
#include "raymath.h"
#include <iostream>
#include <unordered_map>
#include <cstring>

namespace {
Matrix OzzToRayMatrix(const ozz::math::Float4x4& m) {
    Matrix out = {};
    out.m0 = ozz::math::GetX(m.cols[0]);
    out.m1 = ozz::math::GetY(m.cols[0]);
    out.m2 = ozz::math::GetZ(m.cols[0]);
    out.m3 = ozz::math::GetW(m.cols[0]);

    out.m4 = ozz::math::GetX(m.cols[1]);
    out.m5 = ozz::math::GetY(m.cols[1]);
    out.m6 = ozz::math::GetZ(m.cols[1]);
    out.m7 = ozz::math::GetW(m.cols[1]);

    out.m8 = ozz::math::GetX(m.cols[2]);
    out.m9 = ozz::math::GetY(m.cols[2]);
    out.m10 = ozz::math::GetZ(m.cols[2]);
    out.m11 = ozz::math::GetW(m.cols[2]);

    out.m12 = ozz::math::GetX(m.cols[3]);
    out.m13 = ozz::math::GetY(m.cols[3]);
    out.m14 = ozz::math::GetZ(m.cols[3]);
    out.m15 = ozz::math::GetW(m.cols[3]);
    return out;
}
} // namespace

Player::Player(btDiscreteDynamicsWorld* world, const std::string& modelPath, const Vector3& startPos)
    : world(world)
{
    model = LoadModel(modelPath.c_str());

    CalculateModelHeight();
    CreatePhysicsBody(startPos);

    std::cout << "Player constructed!" << std::endl;
    InitOzzAnimation();
}


Player::~Player()
{
    world->removeRigidBody(body);
    delete body;
    delete motionState;
    delete shape;

    UnloadModel(model);
}

void Player::CalculateModelHeight()
{
    BoundingBox box = GetModelBoundingBox(model);
    modelHeight = box.max.y - box.min.y;
}

void Player::CreatePhysicsBody(const Vector3& startPos)
{
    float radius = 0.45f;
    float height = modelHeight * 0.7f;

    shape = new btCapsuleShape(radius, height);

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

Vector3 Player::GetPosition() const
{
    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    btVector3 p = trans.getOrigin();

    return { p.getX(), p.getY(), p.getZ() };
}

void Player::SyncRotation()
{
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));

    // Ignore camera pitch for yaw (no up/down influence).
    forward.y = 0.0f;
    if (Vector3Length(forward) < 0.0001f) forward = {0.0f, 0.0f, 1.0f};
    forward = Vector3Normalize(forward);

    float yaw = atan2f(forward.x, forward.z);   // radians
    btQuaternion q;
    q.setEuler(0, yaw, 0);

    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    trans.setRotation(q);
    body->getMotionState()->setWorldTransform(trans);
}


void Player::Update(float deltaTime)
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
    bool jumpPressed = IsKeyPressed(KEY_SPACE);
    if (jumpPressed) { body->applyCentralImpulse(btVector3(0, 5, 0)); }

          if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
            playerMoveSpeed = 50;
        }else{
            playerMoveSpeed = 10;}


    bool isMoving = Vector3Length(move) > 0.01f;
    move = Vector3Normalize(move);

    btVector3 vel(move.x * playerMoveSpeed, body->getLinearVelocity().y(), move.z * playerMoveSpeed);
    body->setLinearVelocity(vel);

    SyncRotation();

    UpdateOzzAnimation(deltaTime, isMoving, jumpPressed);
}

void Player::Render()
{
    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    btVector3 p = trans.getOrigin();

    Vector3 position = { p.getX(), p.getY(), p.getZ() };

    // Align bottom of model with physics capsule
    position.y -= modelHeight * 0.5f;

    // Render using camera yaw only (keeps model upright).
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    forward.y = 0.0f;
    if (Vector3Length(forward) < 0.0001f) forward = {0.0f, 0.0f, 1.0f};
    forward = Vector3Normalize(forward);
    float yaw = atan2f(forward.x, forward.z);
    Quaternion q = QuaternionFromEuler(0.0f, yaw, 0.0f);

    Vector3 axis = {0.0f, 1.0f, 0.0f};
    float angle = 0.0f;
    QuaternionToAxisAngle(q, &axis, &angle);
    DrawModelEx(model, position, axis, angle * RAD2DEG, {1.0f, 1.0f, 1.0f}, WHITE);

        // Animation is driven by Ozz in UpdateOzzAnimation().
}

void Player::InitOzzAnimation()
{
    std::string skeleton_path = project_dir + "/assets/animrick/skeleton.ozz";
    if (!ozzAnim.LoadSkeleton(skeleton_path.c_str())) {
        std::cerr << "Failed to load Ozz skeleton: " << skeleton_path << std::endl;
        return;
    }

    ozzAnim.LoadAnimation("idle", (project_dir + "/assets/animrick/idle.ozz").c_str());
    ozzAnim.LoadAnimation("walk", (project_dir + "/assets/animrick/walk.ozz").c_str());
    ozzAnim.LoadAnimation("jump", (project_dir + "/assets/animrick/jump.ozz").c_str());

    SetOzzAnimation("idle", true);

    if (model.boneCount <= 0 || model.bones == nullptr || model.bindPose == nullptr) {
        std::cerr << "Model has no bones/bind pose; cannot apply Ozz animation." << std::endl;
        return;
    }

    // Build name map for Ozz joints
    std::unordered_map<std::string, int> jointIndex;
    auto names = ozzAnim.GetSkeleton().joint_names();
    for (int i = 0; i < ozzAnim.GetSkeleton().num_joints(); ++i) {
        jointIndex[names[i]] = i;
    }

    modelBoneToOzz.assign(model.boneCount, -1);
    int matched = 0;
    for (int i = 0; i < model.boneCount; ++i) {
        auto it = jointIndex.find(model.bones[i].name);
        if (it != jointIndex.end()) {
            modelBoneToOzz[i] = it->second;
            matched++;
        }
    }

    std::cout << "Ozz bone mapping: " << matched << "/" << model.boneCount << " bones matched" << std::endl;
    if (matched == 0) {
        std::cout << "Model bone[0..min(10)] names:" << std::endl;
        for (int i = 0; i < model.boneCount && i < 10; ++i) {
            std::cout << "  " << i << ": " << model.bones[i].name << std::endl;
        }
        std::cout << "Ozz joint[0..min(10)] names:" << std::endl;
        for (int i = 0; i < ozzAnim.GetSkeleton().num_joints() && i < 10; ++i) {
            std::cout << "  " << i << ": " << names[i] << std::endl;
        }
    }

    ozzFramePose.resize(model.boneCount);
    ozzFramePosePtrs[0] = ozzFramePose.data();
    ozzModelAnim.frameCount = 1;
    ozzModelAnim.boneCount = model.boneCount;
    ozzModelAnim.bones = model.bones;
    ozzModelAnim.framePoses = ozzFramePosePtrs.data();
    std::strncpy(ozzModelAnim.name, "ozz_runtime", sizeof(ozzModelAnim.name) - 1);
    ozzModelAnim.name[sizeof(ozzModelAnim.name) - 1] = '\0';

    ozzReady = true;
}

void Player::SetOzzAnimation(const std::string& name, bool looping)
{
    if (currentOzzAnim == name) {
        ozzAnim.SetLooping(looping);
        if (!ozzAnim.IsPlaying()) ozzAnim.Play();
        return;
    }
    ozzAnim.SetAnimation(name);
    ozzAnim.SetLooping(looping);
    ozzAnim.Play();
    currentOzzAnim = name;
}

void Player::UpdateOzzAnimation(float deltaTime, bool isMoving, bool jumpPressed)
{
    if (!ozzReady) return;

    if (jumpPressed) {
        SetOzzAnimation("jump", false);
    } else if (currentOzzAnim == "jump") {
        if (!ozzAnim.IsPlaying()) {
            SetOzzAnimation(isMoving ? "walk" : "idle", true);
        }
    } else {
        SetOzzAnimation(isMoving ? "walk" : "idle", true);
    }

    ozzAnim.Update(deltaTime);
    ApplyOzzToModel();
}

void Player::ApplyOzzToModel()
{
    if (!ozzReady) return;

    const auto& matrices = ozzAnim.GetModelMatrices();
    if (matrices.empty()) return;

    for (int i = 0; i < model.boneCount; ++i) {
        int ozzIndex = modelBoneToOzz[i];
        if (ozzIndex >= 0 && ozzIndex < (int)matrices.size()) {
            Matrix m = OzzToRayMatrix(matrices[ozzIndex]);
            MatrixDecompose(m, &ozzFramePose[i].translation, &ozzFramePose[i].rotation, &ozzFramePose[i].scale);
        } else {
            ozzFramePose[i] = model.bindPose[i];
        }
    }

    UpdateModelAnimation(model, ozzModelAnim, 0);
}


Player* player = nullptr;
