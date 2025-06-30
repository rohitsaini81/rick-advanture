#include "physics.h"
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "global_var.h"
#include "camera.h"
#include "objects.h"
#include "script.h"
#include <lua.h>
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
    boxMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    btScalar boxmass = 80.0f;
    btVector3 inertia(0, 0, 0);
    boxShape->calculateLocalInertia(boxmass, inertia);
    btRigidBody::btRigidBodyConstructionInfo boxInfo(boxmass, boxMotion, boxShape, inertia);
    boxBody = new btRigidBody(boxInfo);
    dynamicsWorld->addRigidBody(boxBody);




    // float capsuleRadius = 0.2f;     // half of 0.4m
    // float capsuleHeight = 1.5f;     // total height 1.8m = 1.4 + 0.2*2
    // btScalar mass = 1.0f;

    // playerShape = new btCapsuleShape(capsuleRadius, capsuleHeight);
    btScalar mass = 1.0f;

// Replace capsule with box (half extents = fullSize / 2)
btVector3 halfExtents(0.2f, 0.95f, 0.2f);  // 0.4 x 1.9 x 0.4 box
playerShape = new btBoxShape(halfExtents);
    startTransform.setIdentity();
startTransform.setOrigin(btVector3(0, 5, 0)); // Starting position

playerShape->calculateLocalInertia(mass, inertia);

motionState = new btDefaultMotionState(startTransform);
btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, playerShape, inertia);
playerBody = new btRigidBody(rbInfo);
playerBody->setAngularFactor(btVector3(0, 1, 0));  // allow rotation only on Y-axi
playerBody->setActivationState(DISABLE_DEACTIVATION);
// Add to physics world
dynamicsWorld->addRigidBody(playerBody);


CREATE_ELEM();
CREATE_ELEM();






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




   for(int i=0;i<rigidBodies.size();i++){
        btTransform trans;
        btRigidBody* body = rigidBodies[i];
        if (body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);
        } else {
            trans = body->getCenterOfMassTransform();
        }
        Color randomColor = GetRandomColor();
    
        btVector3 pos = trans.getOrigin();
        DrawCube({pos.getX(), pos.getY(), pos.getZ()}, 2.0f, 2.0f, 2.0f, randomColor);
        std::cout << "Position: " << pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << std::endl;
        // std::cout << "Velocity: " << elem->body->getLinearVelocity().getY() << std::endl;
    }
    


   if (boxBody!=nullptr && boxBody->getMotionState() !=nullptr) {
    btTransform trans;
    boxBody->getMotionState()->getWorldTransform(trans);
    btVector3 pos = trans.getOrigin();
    DrawCube({pos.getX(), pos.getY(), pos.getZ()}, 2.0f, 2.0f, 2.0f, BLUE);

//   std::cout<<"rendering physics 1: " << getPlayerX() << std::endl;
}
    else {
        std::cerr << "[ERROR] boxBody or its motion state is null!" << std::endl;
    }



    // if (IsKeyPressed(KEY_R)) {
    //     LUA_Update();
    // }            DrawRectangle(200, 150, 400, 300, g_scriptColor);



if(playerBody && playerBody->getMotionState()){
btTransform trans;
playerBody->getMotionState()->getWorldTransform(trans);


// Use modelPosition when drawing
btVector3 bulletPos = trans.getOrigin();setPlayerY(bulletPos.getY());setPlayerX(bulletPos.getX());setPlayerZ(bulletPos.getZ());
   // std::cout << "rendering physics: " << bulletPos.getX() << std::endl;

    float capsuleVisualHeight = 1.5f + 2 * 0.2f; // total 1.9
    float halfHeight = capsuleVisualHeight / 2.0f;
    
    Vector3 startPos = { bulletPos.getX(), bulletPos.getY() - halfHeight, bulletPos.getZ() };
    Vector3 endPos   = { bulletPos.getX(), bulletPos.getY() + halfHeight, bulletPos.getZ() };
    
    DrawCapsule(startPos, endPos, 0.2f, 16, 8, RED);
    
}
//this if should be moved to previews one
//movement and jump controls
if(playerBody && playerBody->getLinearVelocity()){

btVector3 vel(0, playerBody->getLinearVelocity().getY(), 0); // keep current vertical velocity (gravity)
if (IsKeyDown(KEY_UP)) std::cout<<"is it fine ? \n";


// Get camera forward and right direction (only XZ for movement)
Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, { 0, 1, 0 }));

// Flatten to XZ plane
forward.y = 0;
right.y = 0;

// Movement vector
btVector3 moveDir(0, 0, 0);

if (IsKeyDown(KEY_W)) moveDir += btVector3(forward.x, 0, forward.z);
if (IsKeyDown(KEY_S)) moveDir -= btVector3(forward.x, 0, forward.z);
if (IsKeyDown(KEY_D)) moveDir += btVector3(right.x, 0, right.z);
if (IsKeyDown(KEY_A)) moveDir -= btVector3(right.x, 0, right.z);

// Keep vertical velocity
btVector3 currentVel = playerBody->getLinearVelocity();
if (moveDir.length2() > 0.0001f) {
    moveDir = moveDir.normalized() * 5.0f;
} else {
    moveDir = btVector3(0, 0, 0);
}


// speed
moveDir.setY(currentVel.getY());

playerBody->setLinearVelocity(moveDir);

if (IsKeyPressed(KEY_SPACE)) {playerBody->applyCentralImpulse(btVector3(0, 5, 0));}}


}







void setPlayerX(float X) { playerX = X; }
void setPlayerY(float Y) { playerY = Y; }
void setPlayerZ(float Z) { playerZ = Z; }
float getPlayerX() { return playerX; }
float getPlayerY() { return playerY; }
float getPlayerZ() { return playerZ; }

