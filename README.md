# Space 3D - Rick Adventure

A 3D game engine featuring Rick and Morty characters with physics-based movement, skeletal animation, and character switching mechanics.

## Features

### Core Gameplay
- **Dual Character System**: Play as Rick or Skeleton with Press **Y** to toggle
- **Physics-Based Movement**: Realistic gravity, collisions, and character physics using Bullet Physics
- **Skeletal Animation**: OZZ animation engine for smooth bone-based character animation
- **Dynamic Camera**: Third-person follow camera with mouse-controlled angles
- **Interactive Controls**:
  - **WASD** - Move character
  - **SPACE** - Jump
  - **SHIFT** - Sprint
  - **Mouse** - Rotate camera
  - **P/O** - Adjust camera distance
  - **U/I** - Adjust camera pitch
  - **Y** - Switch characters
  - **ESC** - Back to menu

### Technical Features
- Multi-platform 3D rendering (OpenGL 3.3)
- Real-time physics simulation with collision detection
- Movement-driven animation (plays when moving, stops when idle)
- Lua scripting for level configuration
- ImGui debugging UI
- Hot-reloadable Lua configuration

## System Requirements

### Build Requirements
- CMake 3.15 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Git (for submodules)
- Linux/macOS/Windows

### Runtime Requirements
- OpenGL 3.3+ compatible GPU
- 512MB RAM minimum
- Mouse and keyboard

## Building

### Quick Start
```bash
# Clone repository
git clone https://github.com/yourusername/space-3d.git
cd space-3d

# Initialize submodules
git submodule update --init --recursive

# Build
mkdir build
cd build
cmake ..
cmake --build .

# Run
./ab_rick
```

### Build Troubleshooting

**Missing dependencies**:
```bash
# Linux (Ubuntu/Debian)
sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxi-dev libxxf86vm-dev

# macOS
brew install cmake

# Windows
# Install Visual Studio Build Tools or use MinGW
```

**Submodule issues**:
```bash
# Update submodules if they fail
git submodule update --init --recursive
```

## Project Structure

```
space-3d/
├── src/
│   ├── main.cpp                  # Game loop and character switching
│   ├── level/
│   │   ├── player/              # Rick character (Player class)
│   │   └── skeleton_player/     # Skeleton character (SkeletonPlayer class)
│   ├── ozz/                     # OZZ animation system
│   ├── Controls/                # Camera and input systems
│   ├── physics.{h,cpp}          # Bullet Physics integration
│   └── [other systems...]
├── .claude/                     # Claude AI documentation
│   ├── PROJECT_CONTEXT.md       # Project overview
│   ├── ARCHITECTURE.md          # System design
│   ├── FILE_STRUCTURE.md        # File organization
│   └── SYSTEMS.md              # Technical systems
├── externals/                   # Third-party libraries
├── CMakeLists.txt              # Build configuration
└── README.md                   # This file
```

## Architecture

### High-Level Design

```
┌────────────────────────────────────────┐
│         MAIN GAME LOOP (60 FPS)        │
└────────────────────────────────────────┘
    │
    ├─ INPUT: Get keyboard/mouse input
    ├─ UPDATE: Apply physics, update character state
    ├─ PHYSICS: Step Bullet world simulation
    ├─ ANIMATION: Update skeletal animation
    ├─ CAMERA: Follow active character
    └─ RENDER: Draw 3D scene
```

### Character System

Both Rick (Player) and Skeleton (SkeletonPlayer) follow the same design pattern:

```
Input (WASD, Space) → Physics Body → Animation → Render
```

**Key Differences**:
| Aspect | Rick | Skeleton |
|--------|------|----------|
| Animation Type | Raylib Frame-based | OZZ Skeletal |
| Physics Body | Dynamic capsule | Dynamic capsule |
| Collision Shape | Cylinder (height=1.26f) | Capsule (height=1.8f) |
| Start Position | {0, 2, 0} | {5, 2, 0} |

### Camera System

Third-person follow camera using spherical coordinates:
- **Pitch** - Vertical angle (-90° to 90°)
- **Yaw** - Horizontal rotation
- **Distance** - Distance from character
- **Target** - Always follows active character

**Coordinate Conversion**:
```
position = target + (distance * cos(pitch) * sin(yaw), distance * sin(pitch), distance * cos(pitch) * cos(yaw))
```

## Technical Details

### Physics Engine
- **Library**: Bullet Physics 3
- **Collision**: Continuous collision detection with SAT (Separating Axis Theorem)
- **Constraints**: Angular factors lock X/Z rotation, allow Y rotation
- **Gravity**: 9.81 m/s² downward

### Animation System
- **Library**: OZZ-Animation
- **Bone Sampling**: Real-time keyframe interpolation
- **Playback**: Time-based with looping support
- **Control**: Movement-driven (play when moving, idle when stopped)

### Rendering
- **Library**: Raylib with OpenGL 3.3
- **Models**: GLB format (Rick model)
- **Skeletal Rendering**: Joint visualization with bone connections

## Scripting

### Lua Configuration
Level objects and parameters are configured in Lua:

```lua
-- assets/script/config.lua
-- Define scene objects, physics properties, etc.
```

**Hot Reloading**: Configuration changes are detected and reloaded automatically.

## Game States

### Menu Screen (screen_number=0)
- Start Game → Begins game
- Options → Settings (TODO)
- Exit → Quit application

### Game Screen (screen_number=1)
- 3D view with active character
- Camera follows character
- Physics and animation running
- Input handling enabled

## Controls Reference

### Character Movement
| Key | Action |
|-----|--------|
| **W** | Move forward |
| **S** | Move backward |
| **A** | Move left |
| **D** | Move right |
| **SPACE** | Jump |
| **SHIFT** | Sprint (2x-5x speed) |

### Camera Control
| Key | Action |
|-----|--------|
| **Mouse X** | Rotate camera horizontally (yaw) |
| **Mouse Y** | Rotate camera vertically (pitch) |
| **P** | Increase camera distance |
| **O** | Decrease camera distance |
| **U** | Increase max pitch |
| **I** | Decrease max pitch |

### Game Control
| Key | Action |
|-----|--------|
| **Y** | Toggle character (Rick ↔ Skeleton) |
| **ESC** | Return to menu |
| **X** | Toggle ImGui debug UI |

## Gameplay Tips

1. **Character Switching**: Press Y at any time to switch between Rick and Skeleton
2. **Camera Angles**: Use mouse to look around, P/O to adjust distance
3. **Movement**: WASD moves in camera-relative directions (forward relative to view)
4. **Sprint**: Hold SHIFT while moving to sprint (5x speed)
5. **Jumping**: Jump momentum is preserved when moving

## Development Workflow

### Adding New Features

**New Character**:
1. Create `src/level/new_character/new_character.{h,cpp}`
2. Mirror Player/SkeletonPlayer structure
3. Update `enum CharacterType` in main.cpp
4. Add instantiation and switching logic

**New Level**:
1. Create `src/level/level2/level2.{h,cpp}`
2. Define objects and physics bodies
3. Add to level loading system

**New Animation**:
1. Add .ozz files to assets/ozz/
2. Update SkeletonAnimation loading paths
3. Modify animation blending if needed

### Debugging

**ImGui UI** (Press X):
- Display current physics state
- Show animation playback time
- View camera parameters
- Real-time parameter adjustment

**Console Output**:
- Character creation logs
- Physics world initialization
- Animation loading status
- Lua script execution feedback

## Performance Metrics

- **Target FPS**: 60
- **Frame Time**: ~16.7ms per frame
- **Physics Updates**: 60 Hz
- **Animation Sampling**: Per frame (2 characters × ~30 joints = ~60 updates/frame)
- **Memory**: ~200MB (with models loaded)

## Known Issues & Limitations

### Current Limitations
- Asset files (models, animations) not included in repository
- Single level system (level1 only)
- No collision between Rick and Skeleton characters
- Jump detection doesn't verify ground contact
- Limited animation set (walking only)

### TODO Features
- [ ] Character animations (idle, running, jumping)
- [ ] NPC system implementation
- [ ] Level editor
- [ ] Sound system
- [ ] Particle effects
- [ ] Multiple levels
- [ ] Save/load system
- [ ] Advanced camera presets

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

### Code Style
- C++17 standards
- 4-space indentation
- Comments for complex logic
- Meaningful variable names

## Documentation

For detailed technical documentation, see:
- `.claude/PROJECT_CONTEXT.md` - Project overview and goals
- `.claude/ARCHITECTURE.md` - System design and architecture patterns
- `.claude/FILE_STRUCTURE.md` - Directory organization and file purposes
- `.claude/SYSTEMS.md` - Technical details of physics, animation, camera systems

## Dependencies

- **raylib** (4.5+) - Graphics and windowing
- **bullet3** (3.x) - Physics simulation
- **ozz-animation** - Skeletal animation
- **imgui** - Debug UI
- **lua** (5.1.5) - Scripting

All dependencies are included as git submodules.

## License

[Specify your license here - MIT, Apache 2.0, etc.]

## Authors

- **Rohit** - Main development

## Acknowledgments

- Raylib community for excellent graphics library
- Bullet Physics developers for robust physics engine
- OZZ-Animation team for skeletal animation system
- Rick and Morty for character inspiration

## Contact & Support

- **Issues**: GitHub Issues for bug reports and feature requests
- **Discussions**: GitHub Discussions for questions and feedback
- **Documentation**: See .claude/ folder for detailed technical docs

## Changelog

### [Latest] - 2025-02-07
- Implemented dual character system (Rick + Skeleton)
- Added Y-key character toggle
- Created SkeletonPlayer class with OZZ animation
- Movement-driven animation control
- Camera follows active character
- Fixed character feet alignment with ground

### Previous Releases
- Initial physics and player system
- Basic menu system
- Lua script integration
