#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include "global_var.h"
#include "camera.h"



extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}
#include "script/script.h"
#include <filesystem>

int main() {

    const std::string scriptPath = project_dir+"src/script/config.lua";




    lua_State* L = luaL_newstate();
    // luaL_openlibs(L);
    luaL_dostring(L, "print('Hello from Lua 5.1.5')");
    run_lua_script(scriptPath.c_str()); 
    load_config(scriptPath.c_str());
    time_t lastModified = getFileLastModifiedTime(scriptPath);
    // Initialization
    InitWindow(800, 600, "Rick and Morty Baby");
    INIT_BEFORE();
    InitPhysics();
        //--->
const std::string Path= "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/rohit/";
const char* modelPath = "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/rohit/src/assets/rick/rick.glb";
    Model model = LoadModel(modelPath);
    // Model plane = LoadModel((project_dir+"src/assets/cube.glb").c_str());
    // if (plane.meshCount == 0) {std::cerr << "Failed to load plane model!" << std::endl;}

    int animCount = 0;
    ModelAnimation* anims = LoadModelAnimations(modelPath, &animCount);
    float animFrameCounter = 0.0f;


    CAM_INIT();
    // Player state
    Vector3 planePos = {2.0f, 2.0f, 2.0f};
    float velocityY = 0.0f;
    float gravity = 0.1f;
    float speed = 5.0f;

    SetTargetFPS(60);
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

    const std::string vshader = project_dir+"src/shaders/phong.vs";
    const std::string fshader = project_dir+"src/shaders/phong.fs";


    Shader shader = LoadShader(vshader.c_str(),fshader.c_str());
SetShaderValue(shader, GetShaderLocation(shader, "lightPos"), &planePos, SHADER_UNIFORM_VEC3);
SetShaderValue(shader, GetShaderLocation(shader, "viewPos"), &camera.position, SHADER_UNIFORM_VEC3);



    while (!WindowShouldClose()) {

        time_t currentModified = getFileLastModifiedTime(scriptPath);
        if (currentModified != lastModified) {
            lastModified = currentModified;
            std::cout << "Reloading Lua script..." << std::endl;
            run_lua_script(scriptPath.c_str());
            load_config(scriptPath.c_str());

        }

        float delta = GetFrameTime();

        // Handle animation
        //--->
        if (animCount > 0) {
            animFrameCounter += 1.0f;
            if (animFrameCounter >= anims[0].frameCount) animFrameCounter = 0;
            UpdateModelAnimation(model, anims[0], (int)animFrameCounter);
        }
//        std::cout<<"what";

        // Update camera to follow player from behind and above
        Vector3 cameraOffset = {-5.0f, 4.0f, 5.0f}; // Offset behind and above
        camera.position = Vector3Add(playerPos, cameraOffset);
        camera.target = playerPos;



        UPDATE_CAMERA();






        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        DrawCube({ 0.0f, 0.0f, 0.0f }, 20.0f, 0.1f, 20.0f, WHITE);
        //--->
        // DrawModel(model, playerPos, 1.0f, WHITE);
        playerPos.x=getPlayerX();
        playerPos.y=(getPlayerY()-1.0f);
        playerPos.z=getPlayerZ();

      render();


        // DrawModelEx(plane, planePos, {1.0f, 0.0f, 0.0f}, -90.0f, {1.0f, 1.0f, 1.0f}, WHITE);
        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("Use WASD to move, SPACE to jump", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Cleanup
        //--->
    if (animCount > 0) UnloadModelAnimations(anims, animCount);
    UnloadModel(model);
    UnloadModel(plane);
    CleanupPhysics();

    CloseWindow();

    return 0;
}
