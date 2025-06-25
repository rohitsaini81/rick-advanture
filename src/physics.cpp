#include "physics.h"
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "global_var.h"
// Bullet globals
btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
btBroadphaseInterface* broadphase = nullptr;
btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
btCollisionDispatcher* dispatcher = nullptr;
btSequentialImpulseConstraintSolver* solver = nullptr;

// Physics objects
btCollisionShape* groundShape = nullptr;
btDefaultMotionState* groundMotion = nullptr;
btRigidBody* groundBody = nullptr;

btCollisionShape* boxShape = nullptr;
btDefaultMotionState* boxMotion = nullptr;
btRigidBody* boxBody = nullptr;

//player physics

btCollisionShape* playerShape = nullptr;
btTransform startTransform;
btDefaultMotionState* motionState = nullptr;
btRigidBody* playerBody = nullptr;




// Player position
float playerX = 0.0f;
float playerY = 0.0f;
float playerZ = 0.0f;

void InitPhysics() {
    // Bullet physics world setup
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

    // Ground setup
    groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
    groundMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundInfo(0.0f, groundMotion, groundShape, btVector3(0, 0, 0));
    groundBody = new btRigidBody(groundInfo);
    dynamicsWorld->addRigidBody(groundBody);

    // Box setup
    boxShape = new btBoxShape(btVector3(1, 1, 1));
    boxMotion = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar mass = 1.0f;
btVector3 inertia(0, 0, 0);
    boxShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo boxInfo(mass, boxMotion, boxShape, inertia);
    boxBody = new btRigidBody(boxInfo);
    dynamicsWorld->addRigidBody(boxBody);




    playerShape = new btCapsuleShape(0.5f, 1.0f); // or btBoxShap
startTransform.setIdentity();
startTransform.setOrigin(btVector3(0, 5, 0)); // Starting position

playerShape->calculateLocalInertia(mass, inertia);

motionState = new btDefaultMotionState(startTransform);
btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, playerShape, inertia);
playerBody = new btRigidBody(rbInfo);

// Add to physics world
dynamicsWorld->addRigidBody(playerBody);

}

void UpdatePhysics(float deltaTime) {
    if (dynamicsWorld) {
        dynamicsWorld->stepSimulation(deltaTime);
    }
}

void CleanupPhysics() {
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;

    delete groundShape;
    delete groundMotion;
    delete groundBody;

    delete boxShape;
    delete boxMotion;
    delete boxBody;
}







void render() {
    if (dynamicsWorld) {
        dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
    }

   if (boxBody && boxBody->getMotionState()) {
        btTransform trans;
        boxBody->getMotionState()->getWorldTransform(trans);
        btVector3 pos = trans.getOrigin();
        // DrawModelEx(plane, { pos.getX(), pos.getY(), pos.getZ() }, {1,0,0}, -90, {1,1,1}, WHITE);
        DrawModel(plane,{ pos.getX(), pos.getY(), pos.getZ() },1.0f,WHITE);

        // DrawCube({ pos.getX(), pos.getY(), pos.getZ() }, 2, 2, 2, RED);
       std::cout<<"rendering physics: " << getPlayerX() << std::endl;
    } else {
        std::cerr << "[ERROR] boxBody or its motion state is null!" << std::endl;
    }



if(playerBody && playerBody->getMotionState()){
btTransform trans;
playerBody->getMotionState()->getWorldTransform(trans);
// Get Bullet position as btVector3
btVector3 bulletPos = trans.getOrigin();setPlayerY(bulletPos.getY());setPlayerX(bulletPos.getX());setPlayerZ(bulletPos.getZ());
    std::cout << "rendering physics: " << bulletPos.getY() << std::endl;
}
//this if should be moved to previews one
//movement and jump controls
if(playerBody && playerBody->getLinearVelocity()){
btVector3 vel(0, playerBody->getLinearVelocity().getY(), 0); // keep current vertical velocity (gravity)

if (IsKeyDown(KEY_W)) vel += btVector3(0, 0, -5);
if (IsKeyDown(KEY_S)) vel += btVector3(0, 0, 5);
if (IsKeyDown(KEY_A)) vel += btVector3(-5, 0, 0);
if (IsKeyDown(KEY_D)) vel += btVector3(5, 0, 0);
playerBody->setLinearVelocity(vel);
if (IsKeyPressed(KEY_SPACE)) {playerBody->applyCentralImpulse(btVector3(0, 5, 0));}}



}







void setPlayerX(float X) { playerX = X; }
void setPlayerY(float Y) { playerY = Y; }
void setPlayerZ(float Z) { playerZ = Z; }
float getPlayerX() { return playerX; }
float getPlayerY() { return playerY; }
float getPlayerZ() { return playerZ; }

