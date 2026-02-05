#include "raylib.h"
#include <btBulletDynamicsCommon.h>
#include <vector>

#include "3dObjects/objects.h"
#include "Controls/camera.h"
#include "ETC/global_var.h"
#include "ozz/skeleton_animation.h"
#include "ozz/skeleton_renderer.h"
#include "ozz/physics_character.h"

// Main
int main() {
    // Initialize project directory and globals
    INIT_BEFORE();

    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Skeleton Animation + Physics");
    SetTargetFPS(60);

    // ---- Setup Camera ----
    Camera3D camera = {0};
    camera.position = {3.0f, 2.0f, 3.0f};
    camera.target = {0.0f, 1.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // ---- Setup Physics World ----
    btDefaultCollisionConfiguration* collision_config =
        new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher =
        new btCollisionDispatcher(collision_config);
    btBroadphaseInterface* broadphase =
        new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver =
        new btSequentialImpulseConstraintSolver();

    btDynamicsWorld* world =
        new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_config);
    world->setGravity(btVector3(0, -9.8f, 0));

    // Create ground
    btCollisionShape* ground_shape = new btBoxShape(btVector3(50, 0.5f, 50));
    btDefaultMotionState* ground_motion = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo ground_info(0, ground_motion, ground_shape);
    btRigidBody* ground = new btRigidBody(ground_info);
    world->addRigidBody(ground);

    // ---- Load Skeleton Animation ----
    SkeletonAnimation anim;

    std::string skeleton_path = project_dir + "/assets/ozz/skeleton.ozz";
    std::string animation_path = project_dir + "/assets/ozz/animation.ozz";

    if (!anim.LoadSkeleton(skeleton_path.c_str())) {
        return 1;
    }
    if (!anim.LoadAnimation(animation_path.c_str())) {
        return 1;
    }

    anim.Play();

    // ---- Setup Skeleton Renderer ----
    SkeletonRenderer renderer;

    // ---- Setup Physics Character ----
    PhysicsCharacter character(world);
    character.InitializeFromSkeleton(anim.GetSkeleton());
    DisableCursor(); // Locks mouse (FPS-style camera)


    float yaw = 0.0f;
    float pitch = 0.0f;
    float camDistance = 5.0f;



    // ---- Main Loop ----
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Update animation
        anim.Update(dt);

        // Update physics character from skeleton
        character.UpdateFromSkeleton(anim.GetModelMatrices());

        // Simulate physics
        world->stepSimulation(dt, 10);




        // Keyboard controls for animation
        if (IsKeyPressed(KEY_SPACE)) {
            anim.IsPlaying() ? anim.Stop() : anim.Play();
        }
        if (IsKeyPressed(KEY_R)) {
            anim.Reset();
        }
        // Apply force continuously while key is held
        if (IsKeyDown(KEY_UP)) {
            character.ApplyForce({0,0,-1}, 50.0f);
            // character.ApplyImpulse({0, 0, -1}, 5.0f);
        }
        if (IsKeyDown(KEY_DOWN)) {
            character.ApplyImpulse({0, 0, 1}, 5.0f);
        }
        if (IsKeyDown(KEY_LEFT)) {
            character.ApplyImpulse({-1, 0, 0}, 5.0f);
        }
        if (IsKeyDown(KEY_RIGHT)) {
            character.ApplyImpulse({1, 0, 0}, 5.0f);
        }

        // ---- Render ----
        BeginDrawing();
        ClearBackground({30, 30, 35, 255});

        BeginMode3D(camera);

        // Draw ground
        DrawPlane({0, -1, 0}, {100, 100}, GRAY);
        DrawGrid(20, 1.0f);

        // Draw skeleton
        renderer.DrawSkeleton(
            anim.GetSkeleton(),
            anim.GetModelMatrices(),
            BLUE,   // joint color
            RED,    // bone color
            0.05f   // joint size
        );

        EndMode3D();

        // Draw UI
        DrawText("Skeleton Animation + Physics", 20, 20, 20, GREEN);
        DrawText("Space: Play/Pause | R: Reset", 20, 45, 16, GRAY);
        DrawText("Arrow Keys: Apply Force | Right Mouse: Rotate Camera", 20, 65, 16, GRAY);

        if (anim.IsPlaying()) {
            DrawText("Playing", 20, 85, 16, GREEN);
        } else {
            DrawText("Stopped", 20, 85, 16, RED);
        }

        DrawFPS(screenWidth - 100, 20);

        EndDrawing();
    }

    // ---- Cleanup ----
    delete world;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collision_config;

    CloseWindow();

    return 0;
}
