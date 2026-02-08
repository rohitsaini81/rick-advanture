#include "player.h"
#include "../../Controls/camera.h"
#include "../../ETC/global_var.h"
#include "raymath.h"
#include <iostream>

Player::Player(btDiscreteDynamicsWorld* world, const std::string& modelPath, const Vector3& startPos)
    : world(world)
{
    model = LoadModel(modelPath.c_str());

    CalculateModelHeight();
    CreatePhysicsBody(startPos);

    std::cout << "Player constructed!" << std::endl;

    animCount = 0;
    anims = LoadModelAnimations(modelPath.c_str(), &animCount);
    float animFrameCounter = 0.0f;


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
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

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
 if (IsKeyPressed(KEY_SPACE)){ body->applyCentralImpulse(btVector3(0, 5, 0));}

          if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
            playerMoveSpeed = 50;
        }else{
            playerMoveSpeed = 10;}


    move = Vector3Normalize(move);

    btVector3 vel(move.x * playerMoveSpeed, body->getLinearVelocity().y(), move.z * playerMoveSpeed);
    body->setLinearVelocity(vel);

    SyncRotation();
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

        if (animCount > 0) {
             animFrameCounter += 1.0f;
             if (animFrameCounter >= anims[0].frameCount) animFrameCounter = 0;
             UpdateModelAnimation(model, anims[0], (int)animFrameCounter);
         }
}


Player* player = nullptr;
