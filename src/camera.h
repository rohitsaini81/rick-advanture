#ifndef CAMERA
#define CAMERA
#include "raylib.h"

extern Camera camera;

extern float pitch;
extern float cameraDistance;
void CAM_INIT();
void UPDATE_CAMERA();

#endif
