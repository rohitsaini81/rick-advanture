# Project Context: Space 3D (Rick Adventure)

## Project Overview

**Space 3D** is a 3D game engine built with Rick and Morty characters. It features physics-based character control, skeletal animation, dynamic camera system, and interactive 3D environments.

**Current Status**: Active development with multi-character support and player control systems.

## Key Goals & Features

### Core Features
- **Physics-Based Movement**: Gravity, collisions, and realistic character physics using Bullet Physics
- **Dual Character System**:
  - Rick (Player) - animated 3D model with WASD controls
  - Skeleton (Animated Character) - OZZ-based skeletal animation with player control
- **Dynamic Camera**: Third-person camera that follows active character with mouse controls
- **Character Switching**: Press Y key to toggle between Rick and Skeleton characters
- **Animation Control**: Movement-driven animation (plays when moving, stops when idle)
- **Scene Management**: Menu system, level loading, scripting support

### In-Development Features
- Lua scripting system for level configuration
- Video player integration
- ImGui-based debugging UI
- NPC system
- Level system with interactive objects

## Tech Stack

### Core Libraries
- **Graphics**: Raylib 4.5+ (OpenGL 3.3)
- **Physics**: Bullet Physics 3
- **Animation**: OZZ-Animation Engine
- **Scripting**: Lua 5.1.5
- **UI**: ImGui
- **Build**: CMake

### Key Dependencies
- **raylib**: 3D rendering and windowing
- **bullet3**: Physics simulation and collision detection
- **ozz-animation**: Skeletal animation and bone transformation
- **lua**: Script execution and configuration

## Recent Implementation (Latest)

### Character Switching System (Y-Key Toggle)
- Added `SkeletonPlayer` class for controllable skeleton characters
- Implemented character type enum (`RICK` / `SKELETON`)
- Camera follows active character with smooth transitions
- Both characters maintain independent positions when inactive

### Animation Improvements
- Animation now responds to player movement (idle vs walking)
- Continuous looping replaced with movement-based playback
- Proper position offsetting for character feet to touch ground

### Camera Enhancements
- Overloaded `UPDATE_CAMERA()` function to accept target position
- Smooth camera transitions when switching characters
- Mouse-controlled camera angles (pitch/yaw)

## Architecture Philosophy

### Input → Physics → Animation Pattern
```
Player Input (WASD, Space, Shift)
    ↓
Physics Engine (Velocity, Forces)
    ↓
Character Position Update
    ↓
Visual Rendering & Animation
```

This pattern ensures:
- Physics-driven movement (not animation-driven)
- Realistic gravity and collision response
- Animation follows character state, not vice versa

### Character Class Pattern
Both `Player` and `SkeletonPlayer` follow the same design:
- Single dynamic capsule physics body
- Input handling for movement
- Render function that positions model at physics body location
- Position offset to align character feet with ground

## Project Dependencies

### Runtime Requirements
- OpenGL 3.3+ compatible GPU
- Linux/macOS/Windows (built on Linux)

### Build Requirements
- CMake 3.15+
- C++17 compiler
- Git (for submodules)
- Audio libraries (for miniaudio support in raylib)

## File Organization

- `src/main.cpp` - Main game loop and character switching logic
- `src/level/player/` - Rick character (Player class)
- `src/level/skeleton_player/` - Skeleton character (SkeletonPlayer class)
- `src/ozz/` - OZZ animation system (SkeletonAnimation, SkeletonRenderer)
- `src/Controls/` - Camera and input systems
- `src/physics.{h,cpp}` - Bullet Physics world and rigid body management
- `src/3dObjects/` - Interactive scene objects
- `src/script/` - Lua integration and configuration

## Known Issues & Limitations

### Current Limitations
- Asset files (models, animations, videos) not included in repository
- Lua scripting system partially implemented
- NPC system not fully integrated
- Physics character system (per-joint kinematic bodies) not used for player control
- Only one level system implemented

### Performance Considerations
- OZZ animation sampling happens every frame
- Physics simulation runs at fixed timestep
- Camera update runs every frame with mouse input handling

## Development Workflow

### Building
```bash
cd /home/rohit/work/sapce_3d
mkdir -p build
cd build
cmake ..
cmake --build .
```

### Running
```bash
./build/ab_rick
```

### Key Controls
- **WASD**: Move active character
- **SPACE**: Jump
- **SHIFT**: Sprint (faster movement)
- **Y**: Toggle between Rick and Skeleton characters
- **P/O**: Adjust camera distance
- **U/I**: Adjust camera pitch (vertical angle)
- **Mouse**: Control camera angle (yaw)
- **ESC**: Return to menu
- **X**: Toggle ImGui debug window

## Next Steps for Development

1. **Character Polish**: Refine animation transitions and movement feel
2. **Asset Integration**: Add comprehensive model and animation library
3. **Level Design**: Build interactive levels with Lua configuration
4. **NPC System**: Implement non-player characters
5. **UI/UX**: Polish menu system and player feedback
6. **Performance**: Profile and optimize physics/animation pipeline
