# File Structure

## Directory Organization

```
sapce_3d/
├── .claude/                    # Claude AI documentation
│   ├── PROJECT_CONTEXT.md     # Project overview and goals
│   ├── ARCHITECTURE.md        # System design and architecture
│   ├── FILE_STRUCTURE.md      # This file
│   └── settings.local.json    # Claude local settings
│
├── src/                        # Source code
│   ├── main.cpp               # Main game loop and entry point
│   ├── physics.{h,cpp}        # Bullet Physics world initialization
│   │
│   ├── level/                 # Character and level systems
│   │   ├── player/            # Rick character (Player class)
│   │   │   ├── player.h
│   │   │   └── player.cpp
│   │   │
│   │   ├── skeleton_player/   # Skeleton character (SkeletonPlayer class)
│   │   │   ├── skeleton_player.h
│   │   │   └── skeleton_player.cpp
│   │   │
│   │   └── level1/            # Level 1 scene
│   │       ├── level.h
│   │       └── level.cpp
│   │
│   ├── ozz/                   # OZZ Animation System
│   │   ├── skeleton_animation.{h,cpp}   # Animation playback and sampling
│   │   ├── skeleton_renderer.{h,cpp}    # Visualization of skeletons
│   │   └── physics_character.{h,cpp}    # Per-joint kinematic bodies (unused for player)
│   │
│   ├── Controls/              # Input and camera systems
│   │   ├── camera.{h,cpp}     # Third-person camera system
│   │   ├── camera.h
│   │   ├── Controller.{h,cpp} # Input controller
│   │   └── Camera.h
│   │
│   ├── 3dObjects/             # Interactive scene objects
│   │   ├── objects.{h,cpp}    # Object definitions
│   │   └── ...
│   │
│   ├── ETC/                   # Utility and global systems
│   │   ├── global_var.{h,cpp}     # Global variables (camera, rendering state)
│   │   ├── Render.{h,cpp}         # Render function implementations
│   │   ├── Loading.{h,cpp}        # Resource loading utilities
│   │   └── ...
│   │
│   ├── script/                # Lua scripting integration
│   │   ├── script.{h,cpp}     # Lua script execution
│   │   └── ...
│   │
│   ├── NPC/                   # Non-player character system (WIP)
│   │   ├── person.{h,cpp}
│   │   └── ...
│   │
│   ├── menu/                  # Menu system
│   │   ├── menu.{h,cpp}       # Menu UI and navigation
│   │   └── ...
│   │
│   ├── video_player/          # Video playback system
│   │   ├── VideoPlayer.{h,cpp}
│   │   └── ...
│   │
│   └── related/               # Miscellaneous utilities
│       └── file.{h,cpp}       # File I/O utilities
│
├── externals/                 # Third-party libraries (git submodules)
│   ├── raylib/                # Graphics library
│   ├── bullet3/               # Physics engine
│   ├── ozz-animation/         # Skeletal animation library
│   ├── imgui/                 # Immediate mode GUI
│   ├── lua-5.1.5/             # Lua scripting engine
│   └── ...
│
├── build/                     # CMake build directory (generated)
│   ├── ab_rick                # Compiled executable
│   └── ...
│
├── assets/                    # Game assets (not included in repo)
│   ├── rick/                  # Rick character model and animations
│   │   └── rick.glb
│   ├── ozz/                   # OZZ animation files
│   │   ├── skeleton.ozz       # Skeleton hierarchy
│   │   └── animation.ozz      # Animation keyframes
│   ├── videos/                # Video files
│   │   └── minecraft.mp4
│   └── ...
│
├── CMakeLists.txt             # CMake build configuration
├── .gitignore                 # Git ignore rules
├── .gitmodules                # Git submodule configuration
├── README.md                  # GitHub README
└── setup.sh                   # Build setup script


## Key Files by Function

### Core Game Loop
- **main.cpp** - Main entry point, character switching, input handling, render loop

### Character Systems
- **level/player/player.{h,cpp}** - Rick character with raylib animation
- **level/skeleton_player/skeleton_player.{h,cpp}** - Skeleton character with OZZ animation

### Physics
- **physics.{h,cpp}** - Bullet Physics world creation and management
- **level/player/player.cpp:CreatePhysicsBody()** - Physics body creation pattern

### Animation
- **ozz/skeleton_animation.{h,cpp}** - OZZ animation playback and sampling
- **ozz/skeleton_renderer.{h,cpp}** - Skeleton visualization
- **ozz/physics_character.{h,cpp}** - Per-joint physics (alternative pattern, unused)

### Camera & Controls
- **Controls/camera.{h,cpp}** - Third-person camera with mouse control
- **Controls/Controller.{h,cpp}** - Input handling

### UI & Rendering
- **ETC/Render.{h,cpp}** - Scene rendering functions
- **menu/menu.{h,cpp}** - Menu system
- **ETC/global_var.{h,cpp}** - Global state (camera params, rendering mode)

### Scripting & Configuration
- **script/script.{h,cpp}** - Lua integration
- **assets/script/config.lua** - Scene configuration (loaded at runtime)

### Utilities
- **related/file.{h,cpp}** - File I/O helpers
- **ETC/Loading.{h,cpp}** - Asset loading functions


## Header Files by Purpose

### Data Structures
- Classes: Player, SkeletonPlayer, SkeletonAnimation, SkeletonRenderer, PhysicsCharacter
- Enums: CharacterType (RICK, SKELETON)

### Interfaces
- **player.h** - Player character interface
- **skeleton_player.h** - Skeleton character interface
- **skeleton_animation.h** - Animation control interface
- **camera.h** - Camera control interface

### System Access
- **physics.h** - Access to bullet physics world
- **global_var.h** - Global variables and rendering state


## Important Implementation Details

### Character Classes
Both Player and SkeletonPlayer follow identical patterns:
```cpp
class Character {
    btRigidBody* body;                      // Physics
    // Animation system (different for each)

    void CreatePhysicsBody(const Vector3& startPos);  // Create capsule
    void Update(float deltaTime);           // Input + Physics
    void Render();                          // Draw at physics position
    Vector3 GetPosition() const;            // For camera
};
```

### Animation Systems
- **Player (Raylib)**: Frame-based ModelAnimation
- **SkeletonPlayer (OZZ)**: Bone matrix-based skeletal animation

### Physics Bodies
All characters use dynamic capsule bodies:
- Radius: 0.45f
- Height: 1.8f (Rick) / 1.8f (Skeleton)
- Mass: 1.0f
- Angular damping: X/Z locked, Y free (allows turning)

### Render Offset
Both characters render with Y offset to align feet with ground:
```cpp
position.y -= modelHeight * 0.5f;  // Or capsuleHeight * 0.5f
```


## Configuration Files

### CMakeLists.txt
- Lists all source files to compile
- Links external libraries (raylib, bullet, ozz, imgui, lua)
- Sets compiler flags and optimization levels
- Key locations: src files listed ~line 100-125

### assets/script/config.lua
- Lua script that defines scene objects
- Loaded at startup by script system
- Hot-reloaded if modified (watched by main loop)

### .gitignore
- Excludes build/ directory
- Excludes binary executables
- Assets typically not included (models, animations, videos)

### .gitmodules
- Specifies external library locations
- raylib, bullet3, ozz-animation, imgui, lua


## Build Output

### Compiled Executable
- **build/ab_rick** - Main game executable
- Built by CMake from all src/ files
- Linked with external libraries from externals/

### Generated Files
- build/CMakeFiles/ - CMake intermediate files
- build/*.a - Static libraries (raylib, bullet, etc.)
- build/*.o - Object files

### Quick Statistics
- ~50 source files total
- ~15 header files in src/
- ~3000 lines main.cpp (includes setup, loop, rendering)
- ~1000 lines physics.cpp
- ~500 lines player.cpp (each character class)
- ~300 lines skeleton_animation.cpp
