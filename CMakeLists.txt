cmake_minimum_required(VERSION 3.10)
project(ab_rick)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Paths ----

# raylib-cpp base directory
set(RAYLIB_CPP_DIR "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp")

# Lua 5.1.5
add_subdirectory(lua-5.1.5)

# ---- Include Directories ----
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/script

    # raylib-cpp and raylib headers
    ${RAYLIB_CPP_DIR}/include
    ${RAYLIB_CPP_DIR}/build/_deps/raylib-src/src

    # Lua 5.1.5 headers
    ${CMAKE_SOURCE_DIR}/lua-5.1.5/src

    # Bullet Physics
    /usr/local/include/bullet
    /usr/local/include
)

# ---- Link Directories ----
#link_directories(
 #   /usr/lib
  #  /usr/local/lib
#)

# ---- Executable ----
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/physics.cpp
    src/global_var.cpp
    src/camera.cpp
    src/objects.cpp
    src/elements.cpp
    src/script/script.cpp
)

# ---- Linking ----
target_link_libraries(${PROJECT_NAME}
    # Your static raylib build
    ${RAYLIB_CPP_DIR}/build/_deps/raylib-build/raylib/libraylib.a

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
)
