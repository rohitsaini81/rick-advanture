cmake_minimum_required(VERSION 3.10)
project(ab_rick)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)





# --- FFmpeg libraries ---
find_library(AVFORMAT_LIB avformat REQUIRED)
find_library(AVCODEC_LIB avcodec REQUIRED)
find_library(AVUTIL_LIB avutil REQUIRED)
find_library(SWSCALE_LIB swscale REQUIRED)

# If swresample or avdevice becomes needed:
# find_library(SWRESAMPLE_LIB swresample)
# find_library(AVDEVICE_LIB avdevice)





# --- Bullet Physics (in externals/bullet3) ---
set(BULLET_BUILD_BULLET2_LIBS ON CACHE BOOL "Build Bullet")
set(BULLET_BUILD_EXTRAS OFF CACHE BOOL "")
set(BULLET_BUILD_UNIT_TESTS OFF CACHE BOOL "")
# ---- Paths ----

# raylib-cpp base directory
# --- Raylib (C library) ---
add_subdirectory(externals/raylib)

# --- raylib-cpp wrapper ---
add_library(raylib_cpp INTERFACE)
target_include_directories(raylib_cpp INTERFACE
    ${CMAKE_SOURCE_DIR}/externals/raylib-cpp/include
)
target_link_libraries(raylib_cpp INTERFACE raylib)


# Lua 5.1.5
add_subdirectory(externals/lua-5.1.5)
add_subdirectory(externals/bullet3)


# ---- Include Directories ----


include_directories(externals/rlImGui)
include_directories(externals/imgui)
include_directories(externals/lua-5.1.5)



    # Add sources
set(IMGUI_SOURCES
   ./externals/imgui/imgui.cpp
   ./externals/imgui/imgui_draw.cpp
   ./externals/imgui/imgui_tables.cpp
   ./externals/imgui/imgui_widgets.cpp
   ./externals/rlImGui/rlImGui.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/script
    ${CMAKE_SOURCE_DIR}/src/ozz

    # raylib-cpp and raylib headers
    ${CMAKE_SOURCE_DIR}/externals/raylib/src        # raylib.h
    ${CMAKE_SOURCE_DIR}/externals/raylib-cpp/include
    # Lua 5.1.5 headers
    ${CMAKE_SOURCE_DIR}/externals/lua-5.1.5/src

    # Bullet Physics
     ${CMAKE_SOURCE_DIR}/externals/bullet3/src

    # OZZ Animation
    ${CMAKE_SOURCE_DIR}/externals/ozz-animation/include
)

# ---- Link Directories ----
#link_directories(
 #   /usr/lib
  #  /usr/local/lib
#)


find_package(Threads REQUIRED)
add_definitions(-DOZZ_BUILD_FUSED)

# ---- Executable ----
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/physics.cpp
    src/ETC/global_var.cpp
    src/Controls/camera.cpp
    src/Controls/Controller.cpp
    src/3dObjects/objects.cpp
    src/3dObjects/elements.cpp
    src/script/script.cpp
    src/NPC/person.cpp
    src/3dObjects/model_obj.cpp
    src/3dObjects/Models.cpp
#   externals/rlImGui/rlImGui.h
#   externals/imgui/imgui.h
    src/related/file.cpp
    src/menu/menu.cpp
    src/level/player/player.cpp
    src/level/level1/level.cpp

    src/video_player/VideoPlayer.cpp
    ${IMGUI_SOURCES}

    # ---- OZZ Animation System ----
    src/ozz/skeleton_animation.cpp
    src/ozz/skeleton_renderer.cpp
    src/ozz/physics_character.cpp

    # ---- OZZ fused sources ----
    externals/ozz-animation/build/src_fused/ozz_animation.cc
    externals/ozz-animation/build/src_fused/ozz_base.cc
    externals/ozz-animation/build/src_fused/ozz_geometry.cc


    )


# ---- OZZ include path (important for fused build) ----

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/externals/ozz-animation/include
    ${CMAKE_SOURCE_DIR}/externals/ozz-animation
)


# ---- Linking ----
target_link_libraries(${PROJECT_NAME}
    # Your static raylib build
#${RAYLIB_CPP_DIR}/build/_deps/raylib-build/raylib/libraylib.a
    raylib_cpp
    # Lua 5.1.5 static lib
    lua_static

    # Bullet Physics
    BulletDynamics
    BulletCollision
    LinearMath

    # System libraries
    pthread m dl rt GL X11

    # Optional stdlib
    stdc++


    ${AVFORMAT_LIB}
    ${AVCODEC_LIB}
    ${AVUTIL_LIB}
    ${SWSCALE_LIB}
)


# Copy assets to build directory
add_custom_command(
    TARGET ab_rick POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
)

# Copy scripts to build directory
add_custom_command(
    TARGET ab_rick POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/script
            ${CMAKE_CURRENT_BINARY_DIR}/script
)

