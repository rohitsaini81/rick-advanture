#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include "3dObjects/objects.h"
#include "Controls/camera.h"
#include "ETC/global_var.h"
#include <iostream>
#include <string>

#include "iostream"
#include "raylib.h"
#include <atomic>
#include <chrono>
#include <thread>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}
#include "script/script.h"
#include <filesystem>

#include "level/player/player.h"
#include "related/file.h"
#include "menu/menu.h"
#include "video_player/VideoPlayer.h"


#include <vector>
#include <iomanip>
#include <sstream>






#include "rlImGui.h"
#include "imgui.h"




namespace fs = std::filesystem;
int main() {
    // Initialization
    SetExitKey(-1); // useless
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(1080, 700, "Rick and Morty Baby");
    SetTargetFPS(60);
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

    Font font = GetFontDefault();
    MenuScreen menu({ "Start Game", "Options", "Exit" }, font);
  //      std::atomic<bool> loadingDone = false;
//std::thread loadingThread(LoadResources, std::ref(loadingDone));
    int dotCounter = 0;
    float timer = 0.0f;
    bool open = false;
    std::string loadingText = "Loading";

            BeginDrawing();
            ClearBackground(RAYWHITE);
            loadingText += ".";
            DrawText(loadingText.c_str(), 350, 200, 30, DARKGRAY);
            EndDrawing();




   INIT_BEFORE();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    loadingText += ".";
    DrawText(loadingText.c_str(), 350, 200, 30, DARKGRAY);
    EndDrawing();

    InitPhysics();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    loadingText += ".";
    DrawText(loadingText.c_str(), 350, 200, 30, DARKGRAY);
    EndDrawing();

    CAM_INIT();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    loadingText += ".";
    DrawText(loadingText.c_str(), 350, 200, 30, DARKGRAY);
    EndDrawing();

    std::string modelPath2 = project_dir + "/assets/rick/rick.glb";
    player = new Player(dynamicsWorld, modelPath2, {0, 2, 0});

    std::string video_path = project_dir +"/assets/videos/minecraft.mp4";
//

  //  VideoPlayer video(video_path);
    std::unique_ptr<VideoPlayer> video = std::make_unique<VideoPlayer>(video_path.c_str());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    loadingText += ".";
    DrawText(loadingText.c_str(), 350, 200, 30, DARKGRAY);
    EndDrawing();
    std::string path = getExecutableDir();

    //--->
    std::string assetsDir = pathJoin(getExecutableDir(), "assets");
    std::string scriptDir = pathJoin(getExecutableDir(), "script");
    const std::string modelPath = assetsDir+"/rick/rick.glb";
    // if (plane.meshCount == 0) {
    //     std::cerr << "Failed to load plane model!" << std::endl;
    // }
    Model model = LoadModel(modelPath.c_str());
    // int animCount = 0;
    // ModelAnimation* anims = LoadModelAnimations(modelPath, &animCount);
    // float animFrameCounter = 0.0f;




    /***************************Lua script loading****************************/
    // Lua is here
    const std::string scriptPath = scriptDir+ "/config.lua";
    std::cout<<scriptPath<<std::endl;
    lua_State* L = luaL_newstate();
    // luaL_openlibs(L);
    luaL_dostring(L, "print('Hello from Lua 5.1.5')");
    run_lua_script(scriptPath.c_str());
    load_config(scriptPath.c_str());
    time_t lastModified = getFileLastModifiedTime(scriptPath);


    // end lua here
    /*********************************************************************/


int screen_number=0;

    rlImGuiSetup(true);  // true = dark style













bool userWantsToClose = false;

 // GAME LOOP HERE
 //

    while (!userWantsToClose) {
        time_t currentModified = getFileLastModifiedTime(scriptPath);
        if (currentModified != lastModified) {
            lastModified = currentModified;
            std::cout << "Reloading Lua script..." << std::endl;
            if (!elementList.empty()) {
                for (int i = 0; i < elementList.size(); i++) {
                    elementList[i]->destroy(dynamicsWorld);
                }
                elementList.clear();
            }
            run_lua_script(scriptPath.c_str());
            load_config(scriptPath.c_str());
        }

        float delta = GetFrameTime();



// UPDATE HERE
control->update(delta);
        // menu screen
        if(screen_number==0){

if(WindowShouldClose()){
userWantsToClose=true;
}




            menu.Update();
        video->Update();   // decode next frame
        int choice = menu.GetSelected();
        if (choice != -1) {
            if (choice == 0) {
                TraceLog(LOG_INFO, "Start Game selected!");
                screen_number=1;
            }
            if (choice == 1) {
                TraceLog(LOG_INFO, "Options selected!");
                screen_number=2;
            }
            if (choice == 2) break; // Exit
        }}else{
            UPDATE_CAMERA();



        // Handle animation
        //--->
        // if (animCount > 0) {
        //     animFrameCounter += 1.0f;
        //     if (animFrameCounter >= anims[0].frameCount) animFrameCounter = 0;
        //     UpdateModelAnimation(model, anims[0], (int)animFrameCounter);
        // }


        if(IsKeyPressed(KEY_ESCAPE)){
            screen_number=0;
        }

        if(IsKeyPressed(KEY_P)){
            cameraDistance++;
        }
        if(IsKeyPressed(KEY_O)){
            cameraDistance--;
        }
        if(IsKeyPressed(KEY_U)){
            maxPitch++;
        }
        if(IsKeyPressed(KEY_I)){
            maxPitch--;

        }





        }




        if (IsKeyPressed(KEY_X)) {
            open = !open;
            if (open) {
                EnableCursor(); // Show and unlock the mouse for ImGui
            }
            else {
                DisableCursor(); // Hide and lock the mouse for game control
            }
            std::cout << "Toggled ImGui window: " << (open ? "OPEN" : "CLOSED") << std::endl;
        }



//  ************RENDER 3D HERE***********
control->render();

        BeginDrawing();
        ClearBackground(BLACK);
        // ClearBackground(RAYWHITE);










        if(screen_number==0){
            video->Render(0, 0);
            menu.Render();
        }



        if(screen_number==1){
            BeginMode3D(camera);
        DrawPlane(Vector3Zero(), (Vector2){10.0, 10.0}, WHITE);
        player->Update(delta);
        player->Render();

        render(delta);
        EndMode3D();
    }

        DrawText("SPACE ENGINE / RICK AND MORTY GM", 10, 10, 20, DARKGRAY);

                rlImGuiBegin();
        // --- ImGui UI ---
        ImGui::Begin("Demo Window");
        ImGui::Text("Hello from ImGui!");
ImGui::SliderFloat("Max Pitch", &maxPitch, 0.0f, 1.0f);
ImGui::SliderFloat("Min Pitch", &minPitch, 0.0f, 1.0f);
        ImGui::End();
         rlImGuiEnd();
        EndDrawing();
    }


    // Cleanup
    //--->
    // if (animCount > 0) UnloadModelAnimations(anims, animCount);
    video.reset();
    CleanupPhysics();
    CloseWindow();

    return 0;
}
