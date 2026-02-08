# Architecture Overview

## System Design

### High-Level Flow

```
┌─────────────────────────────────────────────────────────┐
│                    MAIN GAME LOOP                        │
└─────────────────────────────────────────────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
        ▼                  ▼                  ▼
   UPDATE PHASE      PHYSICS PHASE      RENDER PHASE
        │                  │                  │
    Input Events      Bullet World       OpenGL/Raylib
    Animation Update  Collision          DrawCalls
    Character Pos     Forces/Impulses    Camera Update
        │                  │                  │
        └──────────────────┴──────────────────┘
                           │
                      NEXT FRAME
```

## Core Systems

### 1. Physics System (Bullet Physics 3)
**File**: `src/physics.{h,cpp}`

**Responsibilities**:
- Initialize Bullet dynamics world with gravity
- Manage rigid bodies and collision shapes
- Update physics simulation each frame
- Handle collision detection

**Key Components**:
- `btDiscreteDynamicsWorld` - Physics world
- `btRigidBody` - Dynamic character bodies (Rick, Skeleton)
- `btCollisionShape` - Capsule shapes for characters
- `btCapsuleShape` - Character collision geometry

**Physics Bodies**:
- Rick character: Dynamic capsule at position {0, 2, 0}
- Skeleton character: Dynamic capsule at position {5, 2, 0}
- Ground and static objects: Created from Lua scripts

### 2. Character System

#### Player Class (Rick Character)
**File**: `src/level/player/player.{h,cpp}`

**Design Pattern**: Input → Physics → Render
- Single dynamic capsule physics body
- Raylib ModelAnimation system for walking animation
- Input handling: WASD for movement, Space for jump, Shift for sprint
- Auto-updates animation frame based on movement
- Model positioned at physics body location with Y-offset correction

**Key Methods**:
```cpp
void Update(float deltaTime);  // Handle input, update physics
void Render();                 // Draw model at physics position
Vector3 GetPosition() const;   // Get current position for camera
```

#### SkeletonPlayer Class (Skeleton Character)
**File**: `src/level/skeleton_player/skeleton_player.{h,cpp}`

**Design Pattern**: Same as Player, but with OZZ skeletal animation
- Single dynamic capsule physics body (mirrors Player)
- OZZ animation system for skeletal animation
- Identical input handling to Player
- Animation controlled by movement state (play when moving, stop when idle)
- Skeleton positioned at physics body location with Y-offset correction

**Key Methods**:
```cpp
void Update(float deltaTime);  // Handle input, animation control
void Render();                 // Draw skeleton at physics position
Vector3 GetPosition() const;   // Get current position
void UpdateAnimation(float deltaTime, bool isMoving);  // Control animation playback
```

### 3. Animation System

#### Raylib Animation (Player/Rick)
**Pattern**: Frame-based animation advancement
- Loads GLB model with embedded animations
- Increments frame counter each render call
- Seamless looping

#### OZZ Skeletal Animation (Skeleton)
**Files**: `src/ozz/skeleton_animation.{h,cpp}`, `src/ozz/skeleton_renderer.{h,cpp}`

**Components**:
- `SkeletonAnimation`: Manages playback state and sampling
  - Loads skeleton and animation files
  - Samples bone transformations at current time
  - Converts local transforms to model matrices
- `SkeletonRenderer`: Visualizes skeleton
  - Draws joint spheres at bone positions
  - Draws bone connections
  - Supports transformation (position, rotation, scale)

**Animation Pipeline**:
1. Load skeleton (joint hierarchy) and animation (keyframes)
2. Sample animation at current playback time
3. Generate local transforms for each bone
4. Convert to model-space transforms (LocalToModel job)
5. Render skeleton using model matrices

**Playback Control**:
- `Play()` / `Stop()` - Start/stop animation time advancement
- `Update(dt)` - Advance animation time (only if playing)
- `SetAnimationTime(time)` - Manual time control for specific poses
- `Reset()` - Return to start

### 4. Camera System
**File**: `src/Controls/camera.{h,cpp}`

**Design**: Third-person follow camera with mouse control

**Parameters**:
- `cameraDistance` - Distance from character (default 10)
- `pitch` - Vertical angle (-90 to 90 degrees)
- `yaw` - Horizontal angle (continuous)
- `minPitch` / `maxPitch` - Pitch constraints

**Functionality**:
- Spherical coordinate system: offset is calculated from pitch/yaw
- Mouse delta input controls pitch/yaw
- Camera position = character position + spherical offset
- Camera target = character position (focus point)

**Overloaded Functions**:
- `UPDATE_CAMERA()` - Uses Rick player position
- `UPDATE_CAMERA(Vector3 targetPos)` - Uses provided position (for active character)

### 5. Character Switching System
**File**: `src/main.cpp`

**Implementation**:
```cpp
enum CharacterType { RICK, SKELETON };
CharacterType activeCharacter = RICK;

// In game loop:
if (activeCharacter == RICK) {
    player->Update(delta);
    player->Render();
} else {
    skeletonPlayer->Update(delta);
    skeletonPlayer->Render();
}

// Camera follows active character
Vector3 targetPos = (activeCharacter == RICK)
    ? player->GetPosition()
    : skeletonPlayer->GetPosition();
UPDATE_CAMERA(targetPos);
```

**Behavior**:
- Y key toggles between RICK and SKELETON
- Only active character receives input and physics updates
- Inactive character remains at last position
- Camera smoothly transitions to follow new character

### 6. Input System
**Framework**: Raylib input functions

**Mapped Controls**:
- **WASD** - Directional movement (applied to active character)
- **SPACE** - Jump impulse
- **SHIFT** - Sprint (increase movement speed)
- **Y** - Toggle active character
- **P/O** - Adjust camera distance
- **U/I** - Adjust camera pitch
- **Mouse** - Control camera angle
- **ESC** - Return to menu
- **X** - Toggle ImGui window

### 7. Scene/Level System
**File**: `src/level/level1/level.{h,cpp}`

**Current Implementation**:
- Single level structure
- Loads objects from Lua configuration
- Objects are instantiated with physics bodies
- Static collision geometry from scripts

### 8. Scripting System (Lua)
**File**: `src/script/script.{h,cpp}`

**Integration Points**:
- Load and execute `config.lua` from assets
- Define scene objects and properties
- Hot-reload support (detects file changes)
- Currently used for object initialization

## Data Flow Diagrams

### Character Update Flow
```
Input Check (WASD, Space, Shift)
    ↓
Calculate Movement Vector
    ↓
Apply Physics (Velocity + Gravity)
    ↓
Update Physics World
    ↓
Sync Rotation (Face camera direction)
    ↓
Animation Update (if moving, play; if idle, stop)
    ↓
Render Character at Physics Position
```

### Camera Update Flow
```
Get Mouse Delta
    ↓
Update Yaw from Mouse X (-=deltaX * sensitivity)
    ↓
Update Pitch from Mouse Y (-=deltaY * sensitivity)
    ↓
Clamp Pitch [minPitch, maxPitch]
    ↓
Calculate Spherical Offset
    ↓
Camera Pos = Active Char Pos + Offset
    ↓
Camera Target = Active Char Pos
    ↓
Center Mouse
```

## Class Hierarchy

### Character Classes
```
Player (Rick)
  ├─ btRigidBody (physics)
  ├─ btCapsuleShape (collision)
  ├─ Model (raylib model)
  └─ ModelAnimation[] (raylib animations)

SkeletonPlayer (Skeleton)
  ├─ btRigidBody (physics)
  ├─ btCapsuleShape (collision)
  ├─ SkeletonAnimation (OZZ animation)
  └─ SkeletonRenderer (visualization)
```

### Animation Classes
```
SkeletonAnimation
  ├─ ozz::animation::Skeleton
  ├─ ozz::animation::Animation
  ├─ SamplingJob (sample animation at time t)
  ├─ LocalToModelJob (convert local→model space)
  └─ PlaybackState (is_playing, current_time)

SkeletonRenderer
  ├─ DrawSkeleton() (draws at origin)
  └─ DrawSkeletonTransformed() (draws at position with rotation/scale)
```

## Memory Management

### Lifetime Management
- **Characters**: Created once at startup, deleted at shutdown
- **Physics Bodies**: Added to Bullet world, removed on deletion
- **Animations**: Loaded at character creation, unloaded at destruction
- **Models**: Loaded at character creation, unloaded at destruction

### Resource Pooling
- Static objects in scene are reused
- Animation sampling buffers allocated once per character
- Physics shapes shared between instances where possible

## Threading
**Current**: Single-threaded main loop
- Lua script loading may use threads in future
- Physics world updates sequentially
- No parallel animation sampling

## Performance Considerations

### Critical Paths
1. **Physics Update** - Every frame, O(n) bodies
2. **Animation Sampling** - O(j) joints per character
3. **Rendering** - O(triangles) for models, O(bones*2) for skeleton

### Optimization Opportunities
1. Batch animation sampling for multiple characters
2. Physics body pooling
3. Frustum culling for objects
4. Skeletal animation LOD (level of detail)
5. Physics body sleep states

## Error Handling

### Current Approach
- Log errors to console
- Assert on critical failures (physics world init)
- Graceful degradation for missing assets

### Improvements Needed
- Try-catch for Lua script errors
- Better error messages for missing models
- Animation loading fallback
- Physics simulation validation

## Extensibility Points

### Easy to Add
1. New character types: Create class inheriting character pattern
2. New animations: Add files and update animation lists
3. Camera presets: Add UPDATE_CAMERA overloads
4. Input mapping: Add key checks in main loop
5. Scene objects: Define in Lua or add C++ classes

### Hard to Change
1. Physics engine (would require alternative collision system)
2. Animation system (deeply integrated with OZZ)
3. Character lifecycle (startup/shutdown integrated into main)
4. Render pipeline (tightly coupled with Raylib)
