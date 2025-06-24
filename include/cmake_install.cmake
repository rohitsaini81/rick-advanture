# Install script for directory: /run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/AudioDevice.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/AudioStream.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/AutomationEventList.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/BoundingBox.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Camera2D.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Camera3D.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Color.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/FileData.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/FileText.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Font.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Functions.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Gamepad.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Image.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Keyboard.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Material.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Matrix.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Mesh.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/MeshUnmanaged.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Model.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/ModelAnimation.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Mouse.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Music.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Ray.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/RayCollision.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/RaylibException.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/raylib-cpp-utils.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/raylib-cpp.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/raylib.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/raymath.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Rectangle.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/RenderTexture.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/ShaderUnmanaged.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Shader.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Sound.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Text.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Texture.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/TextureUnmanaged.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Touch.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Vector2.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Vector3.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Vector4.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/VrStereoConfig.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Wave.hpp"
    "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/include/Window.hpp"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/run/media/rohit/8b5b9054-ef1c-4785-aa10-f6a2608b67c8/home/scientist/ArchLinux/work/raylib-cpp/rohit/include/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
