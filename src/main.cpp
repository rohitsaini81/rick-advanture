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



#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#define GLSL_VERSION            330

int main() {

    const std::string scriptPath = project_dir+"src/script/config.lua";




    lua_State* L = luaL_newstate();
    // luaL_openlibs(L);
    luaL_dostring(L, "print('Hello from Lua 5.1.5')");
    run_lua_script(scriptPath.c_str()); 
    load_config(scriptPath.c_str());
    time_t lastModified = getFileLastModifiedTime(scriptPath);




    // Initialization
    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
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
    Vector3 planePos = {2.0f, 4.0f, 2.0f};
    float velocityY = 0.0f;
    float gravity = 0.1f;
    float speed = 5.0f;

    SetTargetFPS(60);
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

    const std::string vshader = project_dir+"src/shaders/phong.vs";
    const std::string fshader = project_dir+"src/shaders/phong.fs";


    Shader shader = LoadShader(TextFormat(vshader.c_str(), GLSL_VERSION),
    TextFormat(fshader.c_str(), GLSL_VERSION));
// Get some required shader locations
shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
// NOTE: "matModel" location name is automatically assigned on shader loading, 
// no need to get the location again if using that uniform name
//shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

// Ambient light level (some basic lighting)
int ambientLoc = GetShaderLocation(shader, "ambient");
// SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
SetShaderValue(shader, ambientLoc, (float[4]){ 0.3f, 0.3f, 0.3f, 1.0f }, SHADER_UNIFORM_VEC4);



    // Create lights
    Light lights[MAX_LIGHTS] = { 0 };
    Color sunLightColor = (Color){ 255, 244, 214, 255 };  // More realistic sunlight
lights[0] = CreateLight(
    LIGHT_DIRECTIONAL,
    (Vector3){ 0, 100, 0 },      // Position (for optional visual debugging)
    (Vector3){ 0, -1, 0 },      // Direction: Downward like sunlight
    Vector3Zero(),             // Target (not used for directional light)
    sunLightColor,                    // Light color
    shader
);

// After (Directional Sun Light)
// lights[0] = CreateLight(LIGHT_DIRECTIONAL,
//     (Vector3){ 0, 10, 0 },     // Position (not used for lighting, just for optional visualization)
//     (Vector3){ 0, -1, 0 },     // Direction: straight down like sunlight
//     YELLOW,
//     shader);

    for (int i = 0; i < model.materialCount; i++) {
        model.materials[i].shader = shader;
    }
    


    while (!WindowShouldClose()) {

        time_t currentModified = getFileLastModifiedTime(scriptPath);
        if (currentModified != lastModified) {
            lastModified = currentModified;
            std::cout << "Reloading Lua script..." << std::endl;
            run_lua_script(scriptPath.c_str());
            load_config(scriptPath.c_str());

        }

        float delta = GetFrameTime();
        // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        
    
           // Update light values (actually, only enable/disable them)
           for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);
           //----------------------------------------------------------------------------------
   
           // Draw
           //--------


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
        // ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        BeginShaderMode(shader);
        DrawCube({ 0.0f, 0.0f, 0.0f }, 20.0f, 0.1f, 20.0f, WHITE);
        
        DrawPlane(Vector3Zero(), (Vector2) { 10.0, 10.0 }, WHITE);


        DrawModel(model, playerPos, 1.0f, WHITE);



      render();



        EndShaderMode();



        for (int i = 0; i < MAX_LIGHTS; i++) {
            if (lights[i].type == LIGHT_POINT) {
                if (lights[i].enabled)
                    DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
                else
                    DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
            } else if (lights[i].type == LIGHT_DIRECTIONAL) {
                // Visualize directional light with a line (like sunlight)
                Vector3 endPoint = Vector3Add(lights[i].position, Vector3Scale(lights[i].direction, 2.0f));
                DrawLine3D(lights[i].position, endPoint, lights[i].color);
            }
        }
        
        


        playerPos.x=getPlayerX();
        playerPos.y=(getPlayerY()-1.0f);
        playerPos.z=getPlayerZ();



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
    UnloadShader(shader);   // Unload shader
    CloseWindow();

    return 0;
}
