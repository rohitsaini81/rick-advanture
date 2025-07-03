#include "camera.h"
#include "raylib.h"
#include "raymath.h"
#include "global_var.h"
#include "iostream"
Camera camera = {0};
float pitch = 15.0f;  // Slightly looking down
float cameraDistance = 10.0f;
void CAM_INIT(){
        // Camera setup
        camera.projection = CAMERA_PERSPECTIVE;
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 45.0f;
       DisableCursor(); // Hide and lock cursor

}

float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
const float mouseSensitivity = 0.003f;
Vector2 mouseDelta = GetMouseDelta();
void UPDATE_CAMERA(){

// Mouse movement
Vector2 mouseDelta = GetMouseDelta();
yaw -= mouseDelta.x * 0.1f;
pitch -= mouseDelta.y * 0.1f;

// Clamp pitch to avoid flipping
if (pitch > 89.0f) pitch = 89.0f;
if (pitch < -89.0f) pitch = -89.0f;
if (pitch <=22.0f) pitch =22.0f;
if (pitch >=80.0f) pitch = 80.0f;
// std::cout<<"pitch"<<pitch<<std::endl;
// Convert spherical coordinates to cartesian
Vector3 cameraOffset;
cameraOffset.x = cameraDistance * cosf(DEG2RAD * pitch) * sinf(DEG2RAD * yaw);
cameraOffset.y = cameraDistance * sinf(DEG2RAD * pitch);
cameraOffset.z = cameraDistance * cosf(DEG2RAD * pitch) * cosf(DEG2RAD * yaw);

// Update camera
camera.position = Vector3Add(playerPos, cameraOffset);
camera.target = playerPos;
camera.up = { 0.0f, 1.0f, 0.0f };
SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);



}
