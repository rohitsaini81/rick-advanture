

cmake_minimum_required(VERSION 3.10)
project(ab_rick)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to your local raylib-cpp clone
set(RAYLIB_CPP_DIR "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/")

# Include raylib-cpp headers
include_directories(${RAYLIB_CPP_DIR}/include)

# Include raylib headers
include_directories(${RAYLIB_CPP_DIR}/build/_deps/raylib-src/src)


include_directories(/usr/local/include/bullet)
include_directories(/usr/local/include)


# Add executable
add_executable(${PROJECT_NAME}
     src/main.cpp
     src/physics.cpp
     src/global_var.cpp
)

# Link static raylib library (with full path)
target_link_libraries(${PROJECT_NAME}
    ${RAYLIB_CPP_DIR}/build/_deps/raylib-build/raylib/libraylib.a


    # Bullet Physics
    BulletDynamics
    BulletCollision
    LinearMath

    
    pthread m dl rt GL X11
)
