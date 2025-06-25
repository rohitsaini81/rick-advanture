#include "global_var.h"
#include <iostream>
#include "raylib.h"
const std::string project_dir = "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/rohit/";
Model plane = {0};
void INIT_BEFORE (){
    std::cout<<"path"<<project_dir;
    plane = LoadModel((project_dir+"src/assets/cube.glb").c_str());
    if (plane.meshCount == 0) {std::cerr << "Failed to load plane model!" << std::endl;}

}