#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include "global_var.h"
#include "camera.h"
int main() {
    //controls control;
    // Initialization
    InitWindow(800, 600, "Rick and Morty Baby");
    INIT_BEFORE();
    InitPhysics();
// const std::string Path= "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/rohit/";
const char* modelPath = "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/rohit/src/assets/rick/rick.glb";
    Model model = LoadModel(modelPath);
    // Model plane = LoadModel((project_dir+"src/assets/cube.glb").c_str());
    // if (plane.meshCount == 0) {std::cerr << "Failed to load plane model!" << std::endl;}

    int animCount = 0;
    ModelAnimation* anims = LoadModelAnimations(modelPath, &animCount);
    float animFrameCounter = 0.0f;


    CAM_INIT();
    // Player state
    Vector3 planePos = {0.0f, 0.0f, 0.0f};
    float velocityY = 0.0f;
    float gravity = 0.1f;
    float speed = 5.0f;

    SetTargetFPS(60);




SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);



    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Handle animation
        if (animCount > 0) {
            animFrameCounter += 1.0f;
            if (animFrameCounter >= anims[0].frameCount) animFrameCounter = 0;
            UpdateModelAnimation(model, anims[0], (int)animFrameCounter);
        }
//        std::cout<<"what";

        // Update camera to follow player from behind and above
        // Vector3 cameraOffset = {-5.0f, 4.0f, 5.0f}; // Offset behind and above
        // camera.position = Vector3Add(playerPos, cameraOffset);
        // camera.target = playerPos;



        UPDATE_CAMERA();






        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        DrawModel(model, playerPos, 1.0f, WHITE);
        playerPos.x=getPlayerX();
        playerPos.y=(getPlayerY()-1.0f);
        playerPos.z=getPlayerZ();

      render();


        // DrawModelEx(plane, planePos, {1.0f, 0.0f, 0.0f}, -90.0f, {1.0f, 1.0f, 1.0f}, WHITE);
        // DrawCube({ 1.0f, 1.0f,1.0f }, 2, 2, 2, RED);
        DrawGrid(10, 1.0f);

        EndMode3D();
        //DrawCube(playerPos, 1.0f, 2.0f, 1.0f, RED); // Temporary debug cube

        DrawText("Use WASD to move, SPACE to jump", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Cleanup
    if (animCount > 0) UnloadModelAnimations(anims, animCount);
    UnloadModel(model);
    UnloadModel(plane);
    CleanupPhysics();

    CloseWindow();

    return 0;
}
