# Quick Start Guide for Claude

This file helps Claude Code quickly understand the project when starting a new session.

## What Is This Project?

**Space 3D** is a 3D game engine featuring Rick and Morty characters with:
- Two playable characters (Rick + Skeleton) - toggle with Y key
- Physics-based movement using Bullet Physics
- Skeletal animation using OZZ animation engine
- Third-person camera system
- Lua scripting support

## Project Location
```
/home/rohit/work/sapce_3d/
```

## Key Files to Know

### Game Loop & Character Switching
- **src/main.cpp** - Main game loop, character switching logic (~400 lines of code)
  - Lines 98-99: Player initialization
  - Lines 101-107: SkeletonPlayer initialization
  - Lines 275-280: Y-key toggle handler
  - Lines 328-337: Render active character only

### Character Classes
- **src/level/player/player.{h,cpp}** - Rick character with Raylib animation
  - 150 lines, simple to understand, good reference for SkeletonPlayer
- **src/level/skeleton_player/skeleton_player.{h,cpp}** - Skeleton with OZZ animation
  - New class (just implemented), mirrors Player structure

### Animation System
- **src/ozz/skeleton_animation.{h,cpp}** - OZZ animation playback
  - Key methods: Play(), Stop(), Update(dt), SetAnimationTime()
  - Samples bone transformations, converts to model matrices
- **src/ozz/skeleton_renderer.{h,cpp}** - Visualizes skeleton (joint spheres + bones)

### Physics & Camera
- **src/physics.{h,cpp}** - Bullet Physics world setup
- **src/Controls/camera.{h,cpp}** - Third-person camera system
  - `UPDATE_CAMERA()` - Original version (uses Rick)
  - `UPDATE_CAMERA(Vector3 targetPos)` - New version (uses active character)

## Build & Run

```bash
cd /home/rohit/work/sapce_3d

# Build
mkdir -p build && cd build && cmake .. && cmake --build .

# Run
./ab_rick
```

## Recent Changes (Latest Implementation)

### Character Switching System ✅
Added Y-key toggle between Rick and Skeleton:
- Enum: `enum CharacterType { RICK, SKELETON }`
- Only active character updated/rendered
- Camera follows active character smoothly

### SkeletonPlayer Class ✅
New class for controllable skeleton character:
- Mirrors Player class structure
- Uses OZZ skeletal animation
- Movement-driven animation (plays when moving, stops when idle)
- Physics body automatically positioned

### Animation Control ✅
Movement-based animation:
- Animation plays only when WASD keys pressed
- Stops and resets when idle
- Added `SetAnimationTime()` to SkeletonAnimation

### Position Fix ✅
Both characters' feet now touch ground:
- Applied Y-offset equal to half capsule height
- Similar to Player class approach

## Architecture in 30 Seconds

```
Input (WASD, Y key)
    ↓
Active Character Update
    ├─ Apply Physics (velocity, gravity)
    ├─ Update Animation (based on movement)
    └─ Get Position
    ↓
Camera Follows Active Character
    ↓
Render Active Character
```

## How to Add Something

### New Character Type
1. Create `src/level/new_char/new_char.{h,cpp}`
2. Copy structure from `SkeletonPlayer` or `Player`
3. Add to `CharacterType` enum in main.cpp
4. Instantiate in main.cpp lines ~101-107
5. Add case in switch statement around line 328

### New Input/Control
1. Add `IsKeyPressed/IsKeyDown()` in main.cpp game loop
2. Apply effect to active character
3. Or modify active character's Update() method

### New Animation
1. Add OZZ files: `assets/ozz/newanimation.ozz`
2. Update SkeletonAnimation loading or add new animation loader
3. Connect to SkeletonPlayer animation switching

## Common Tasks

### Debug Character Movement
- Check `Player::Update()` (line 92-128) or `SkeletonPlayer::Update()`
- Verify WASD key handling
- Check physics body velocity application
- Check collision with ground

### Debug Animation
- Check `SkeletonPlayer::UpdateAnimation()`
- Verify `anim.IsPlaying()` state
- Check movement detection: `Vector3Length(move) > 0.01f`
- Ensure `anim.Update(deltaTime)` called when playing

### Debug Camera
- Check `UPDATE_CAMERA()` in Controls/camera.cpp
- Verify `targetPos = skeletonPlayer->GetPosition()`
- Check spherical coordinate calculation
- Mouse delta should update pitch/yaw

### Debug Physics
- Check `CreatePhysicsBody()` in character class
- Verify capsule dimensions: radius=0.45f, height varies
- Check angular constraints: `setAngularFactor(0, 1, 0)`
- Verify gravity applied to world

## Code Patterns to Follow

### Character Update Pattern
```cpp
void Update(float deltaTime) {
    // 1. Get input
    Vector3 move = GetInputDirection();

    // 2. Apply forces
    body->setLinearVelocity(...);

    // 3. Update rotation/animation
    SyncRotation();
    UpdateAnimation(deltaTime, isMoving);
}
```

### Capsule Physics Pattern
```cpp
shape = new btCapsuleShape(radius, height);
btTransform startTransform;
startTransform.setOrigin(btVector3(x, y, z));
motionState = new btDefaultMotionState(startTransform);
btRigidBody::btRigidBodyConstructionInfo rbInfo(...);
body = new btRigidBody(rbInfo);
body->setAngularFactor(btVector3(0, 1, 0));  // Only Y rotation
world->addRigidBody(body);
```

### Animation Control Pattern
```cpp
if (isMoving) {
    if (!anim.IsPlaying()) anim.Play();
    anim.Update(deltaTime);
} else {
    if (anim.IsPlaying()) anim.Stop();
    anim.Reset();
}
```

## Performance Notes

- Physics updates: 60 Hz
- Animation sampling: Per frame (expensive for many bones)
- Camera updates: Per frame with mouse input
- Rendering: Per frame with Raylib

**Optimization opportunities**:
- Batch multiple animations
- Physics body sleep states
- Frustum culling
- Animation LOD

## Testing Checklist

- [ ] Character switches with Y key
- [ ] Both characters can move independently
- [ ] Camera follows active character
- [ ] Animation plays when moving, stops when idle
- [ ] Character feet touch ground
- [ ] Jump works with SPACE
- [ ] Sprint works with SHIFT
- [ ] Camera angles work (mouse, P/O, U/I)
- [ ] Menu works (ESC to return, start game)

## Important Locations in Code

| Task | Location |
|------|----------|
| Character switching logic | main.cpp:274-280 |
| Active character render | main.cpp:328-337 |
| Camera following | main.cpp:242-247 |
| Player input | Player::Update() & SkeletonPlayer::Update() |
| Animation control | SkeletonPlayer::UpdateAnimation() |
| Physics setup | physics.cpp:InitPhysics() |
| Camera math | Controls/camera.cpp:25-58 |

## Common Errors & Fixes

**"undefined reference to `SkeletonPlayer::...`"**
- Add `src/level/skeleton_player/skeleton_player.cpp` to CMakeLists.txt

**"Animation doesn't play"**
- Check `anim.IsPlaying()` in UpdateAnimation
- Verify `anim.Play()` is called
- Check `anim.Update(dt)` is executed

**"Character flying in air"**
- Check Y offset in Render: `position.y -= capsuleHeight * 0.5f`
- Verify physics body height matches expected height

**"Camera stuck"**
- Check `UPDATE_CAMERA(targetPos)` is being called
- Verify target position is non-zero
- Check mouse not locked (DisableCursor/EnableCursor)

## Next Steps

1. **Test current implementation** - Play the game, verify character switching
2. **Expand animations** - Add more OZZ animations, blend between states
3. **Add levels** - Create level2, level3, etc.
4. **Improve polish** - Better character models, animations, UI
5. **Add gameplay** - Objectives, enemies, pickups, etc.

## Documentation Files

- **PROJECT_CONTEXT.md** - Project goals and tech stack
- **ARCHITECTURE.md** - Detailed system design and patterns
- **FILE_STRUCTURE.md** - Complete directory organization
- **SYSTEMS.md** - Technical deep dive (physics, animation, camera)
- **QUICK_START.md** - This file
