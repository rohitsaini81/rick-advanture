# Technical Systems Documentation

## Physics System (Bullet Physics)

### World Setup
```cpp
// Initialize physics world
InitPhysics()  // src/physics.cpp
  ├─ Create btDefaultCollisionConfiguration
  ├─ Create btCollisionDispatcher
  ├─ Create btBroadphaseInterface (AABB tree)
  ├─ Create btSequentialImpulseConstraintSolver
  ├─ Create btDiscreteDynamicsWorld
  └─ Set gravity: (0, -9.81, 0)
```

### Character Physics Bodies

**Rick Character**:
- Type: Dynamic rigid body (fully simulated)
- Shape: btCapsuleShape(radius=0.45f, height=1.8f*0.7f)
- Mass: 1.0f
- Position: {0, 2, 0}
- Angular constraints: Lock X/Z, free Y rotation
- Activation: DISABLE_DEACTIVATION (always active)

**Skeleton Character**:
- Type: Dynamic rigid body (fully simulated)
- Shape: btCapsuleShape(radius=0.45f, height=1.8f)
- Mass: 1.0f
- Position: {5, 2, 0}
- Angular constraints: Lock X/Z, free Y rotation
- Activation: DISABLE_DEACTIVATION (always active)

**Ground & Static Objects**:
- Type: Static bodies (infinite mass)
- Shape: Defined per object (boxes, planes)
- Loaded from Lua configuration

### Physics Update Loop
```cpp
// Each frame in main loop
float dt = GetFrameTime();  // ~0.0167s at 60 FPS

// 1. Apply forces from character input
btVector3 velocity = ...  // From WASD input
body->setLinearVelocity(velocity);

// 2. Physics world steps
dynamicsWorld->stepSimulation(dt, substeps);
  ├─ Collision detection (AABB broad phase → shape collision)
  ├─ Generate contact manifolds
  ├─ Solve constraints (velocity, penetration)
  └─ Integrate physics (position += velocity * dt)

// 3. Get updated position
btTransform trans;
body->getMotionState()->getWorldTransform(trans);
Vector3 newPos = ExtractPosition(trans);
```

### Collision Management

**Collision Pairs**:
- Rick ↔ Ground (prevent falling through)
- Rick ↔ Static objects (prevent penetration)
- Skeleton ↔ Ground (prevent falling through)
- Skeleton ↔ Static objects (prevent penetration)

**No Collision**:
- Rick ↔ Skeleton (allow passing through for now)

### Forces & Impulses

**Gravity**: Applied continuously (-9.81 m/s² in Y)
**Jump**: Applied impulse when SPACE pressed
```cpp
if (IsKeyPressed(KEY_SPACE)) {
    body->applyCentralImpulse(btVector3(0, 5, 0));
}
```

**Movement**: Velocity applied directly (no friction simulation)
```cpp
float moveSpeed = (IsKeyDown(KEY_SHIFT)) ? 50 : 10;
btVector3 vel(moveX * moveSpeed, body->getLinearVelocity().y(), moveZ * moveSpeed);
body->setLinearVelocity(vel);
```

### Physics Debugging
- ImGui UI shows physics state (ImGui window - toggle with X key)
- Collision shapes can be visualized (not currently implemented)


## Animation System (OZZ)

### OZZ Pipeline Overview
```
skeleton.ozz              animation.ozz
(bone hierarchy)          (keyframes)
      │                        │
      └────────────────┬───────┘
                       │
            SkeletonAnimation
                       │
    ┌──────────────────┼──────────────────┐
    │                  │                  │
Sampling Job       LocalToModel Job   Output Buffers
(sample keyframes) (local→model space) (matrices)
    │                  │                  │
    └──────────────────┴──────────────────┘
                       │
            model_matrices_[]
            (4x4 transformation per bone)
                       │
            SkeletonRenderer::Draw()
```

### Animation Classes

#### SkeletonAnimation
**File**: `src/ozz/skeleton_animation.{h,cpp}`

**Members**:
```cpp
ozz::animation::Skeleton skeleton_;              // Joint hierarchy
ozz::animation::Animation animation_;            // Keyframe data
std::vector<ozz::math::SoaTransform> local_transforms_;  // Per-joint local space
std::vector<ozz::math::Float4x4> model_matrices_;        // Per-joint model space

float current_time_;                             // Playback position
float playback_speed_;                           // Animation speed multiplier
bool is_playing_;                                // Playback state
bool is_looping_;                                // Loop at end
```

**Key Methods**:
```cpp
// Loading
bool LoadSkeleton(const char* path);      // Load .ozz skeleton file
bool LoadAnimation(const char* path);     // Load .ozz animation file

// Playback control
void Play();                              // Start playback
void Stop();                              // Pause playback
void Reset();                             // Return to start (time=0)
void SetAnimationTime(float time);        // Set specific time (for poses)
void Update(float dt);                    // Advance time if playing

// Accessors
const std::vector<ozz::math::Float4x4>& GetModelMatrices() const;
float GetCurrentTime() const;
float GetDuration() const;
bool IsPlaying() const;
```

**Update Process**:
```cpp
void SkeletonAnimation::Update(float dt) {
    if (!is_playing_) return;

    // 1. Advance time
    current_time_ += dt * playback_speed_;

    // 2. Handle looping
    if (is_looping_) {
        ratio = fmod(current_time_, animation_.duration()) / animation_.duration();
    } else {
        if (current_time_ >= animation_.duration()) {
            current_time_ = animation_.duration();
            is_playing_ = false;
        }
        ratio = current_time_ / animation_.duration();
    }

    // 3. Sample animation at this time
    sampling_job_.ratio = ratio;
    sampling_job_.Run();  // Fill local_transforms_

    // 4. Convert to model space
    ltm_job_.Run();  // Fill model_matrices_
}
```

#### SkeletonRenderer
**File**: `src/ozz/skeleton_renderer.{h,cpp}`

**Rendering Functions**:
```cpp
void DrawSkeleton(
    const ozz::animation::Skeleton& skeleton,
    const std::vector<ozz::math::Float4x4>& model_matrices,
    Color joint_color = BLUE,
    Color bone_color = RED,
    float joint_size = 0.03f
);
// Draws skeleton at origin with joint spheres and bone lines

void DrawSkeletonTransformed(
    const ozz::animation::Skeleton& skeleton,
    const std::vector<ozz::math::Float4x4>& model_matrices,
    Vector3 position,
    Quaternion rotation,
    float scale = 1.0f,
    Color joint_color = BLUE,
    Color bone_color = RED
);
// Draws skeleton at specified position with rotation/scale
```

**Drawing Process**:
```
For each joint in skeleton:
  1. Extract position from model_matrix[joint]
  2. Draw sphere at that position (joint visualization)
  3. Draw line from joint to parent joint (bone visualization)
```

### Animation Control in SkeletonPlayer

**Movement-Driven Animation**:
```cpp
void SkeletonPlayer::UpdateAnimation(float deltaTime, bool isMoving) {
    if (isMoving) {
        // Playing while moving
        if (!anim.IsPlaying()) {
            anim.Play();  // Start if stopped
        }
        anim.Update(deltaTime);  // Advance animation
    } else {
        // Stopped
        if (anim.IsPlaying()) {
            anim.Stop();  // Stop playback
            anim.Reset();  // Reset to first frame
        }
    }
}
```

**Motion Detection**:
```cpp
// In SkeletonPlayer::Update()
Vector3 move = {0, 0, 0};
if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);

bool isMoving = Vector3Length(move) > 0.01f;
UpdateAnimation(deltaTime, isMoving);
```

### OZZ File Format

**skeleton.ozz**:
- Binary format containing joint hierarchy
- Each joint has: name, parent index, local transform
- Loaded with: `ozz::io::IArchive`

**animation.ozz**:
- Binary format containing keyframe animation
- Tracks: rotation, translation, scale per joint
- Sampled at any normalized time [0, 1]
- Duration in seconds stored in animation object


## Camera System

### Camera State
```cpp
extern Camera camera;        // Raylib Camera3D
extern float pitch;          // Vertical angle (-90 to 90)
extern float yaw;            // Horizontal angle (0 to 360)
extern float cameraDistance; // Distance from target
extern float minPitch, maxPitch;  // Pitch constraints
```

### Update Pipeline
**File**: `src/Controls/camera.cpp:UPDATE_CAMERA()`

```cpp
void UPDATE_CAMERA(Vector3 targetPos) {
    // 1. Get mouse input
    Vector2 mouseDelta = GetMouseDelta();

    // 2. Update angles
    yaw -= mouseDelta.x * 0.1f;           // Horizontal rotation
    pitch -= mouseDelta.y * 0.1f;         // Vertical rotation

    // 3. Constrain pitch
    if (pitch <= minPitch) pitch = minPitch;
    if (pitch >= maxPitch) pitch = maxPitch;

    // 4. Calculate spherical offset
    float radiusAtPitch = cameraDistance * cosf(DEG2RAD * pitch);
    camera.position.x = targetPos.x + radiusAtPitch * sinf(DEG2RAD * yaw);
    camera.position.y = targetPos.y + cameraDistance * sinf(DEG2RAD * pitch);
    camera.position.z = targetPos.z + radiusAtPitch * cosf(DEG2RAD * yaw);

    // 5. Set camera focus
    camera.target = targetPos;      // Look at character
    camera.up = {0, 1, 0};          // Up vector

    // 6. Reset mouse to center
    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
}
```

### Spherical Coordinate System

**Parameters**:
- `distance` - Distance from target (cameraDistance)
- `pitch` - Angle up/down from horizontal (-90 to 90°)
- `yaw` - Angle around vertical axis (0 to 360°)

**Conversion to Cartesian**:
```
radius_at_pitch = distance * cos(pitch)
offset.x = radius_at_pitch * sin(yaw)
offset.y = distance * sin(pitch)
offset.z = radius_at_pitch * cos(yaw)

camera.position = targetPos + offset
```

**Example Angles**:
- pitch=0°, yaw=0° → Camera to the right side
- pitch=45°, yaw=0° → Camera elevated and to the right
- pitch=-45°, yaw=180° → Camera below and behind
- pitch=90°, yaw=any → Camera directly above

### Camera Controls

**Keyboard**:
- **P** - Increase `cameraDistance` (move away)
- **O** - Decrease `cameraDistance` (move closer)
- **U** - Increase `maxPitch` (allow higher angles)
- **I** - Decrease `maxPitch` (constrain lower)

**Mouse**:
- **X Axis** - Update `yaw` (horizontal pan)
- **Y Axis** - Update `pitch` (vertical pan)
- **Center** - Resets each frame

### Character Following

**Before Character Switching**:
```cpp
camera.target = player->GetPosition();  // Fixed in camera.cpp
```

**After Character Switching**:
```cpp
// In main.cpp
Vector3 targetPos = (activeCharacter == RICK)
    ? player->GetPosition()
    : skeletonPlayer->GetPosition();
UPDATE_CAMERA(targetPos);  // Use overloaded version
```


## Input System

### Input Mapping

**Character Movement** (Applied to active character):
```cpp
WASD - Directional movement (forward/back/left/right)
SPACE - Jump impulse
SHIFT - Sprint (increase movement speed from 10 to 50)
```

**Camera Control**:
```cpp
Mouse X/Y - Rotate camera (yaw/pitch)
P - Camera distance increase
O - Camera distance decrease
U - Max pitch increase
I - Max pitch decrease
```

**Game Control**:
```cpp
Y - Toggle active character (RICK ↔ SKELETON)
ESC - Return to menu
X - Toggle ImGui debug window
```

### Input Detection

**Raylib Functions Used**:
```cpp
IsKeyPressed(KEY_CODE)   // True once when key transitions to pressed
IsKeyDown(KEY_CODE)      // True while key is held
IsKeyReleased(KEY_CODE)  // True once when key transitions to released
GetMouseDelta()          // Change in mouse position since last frame
GetScreenWidth/Height()  // Window dimensions
```

### Movement Calculation

**Direction Vector**:
```cpp
Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

// Flatten to horizontal plane
forward.y = 0;
right.y = 0;

// Accumulate input
Vector3 move = {0, 0, 0};
if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);

// Normalize to prevent diagonal speedup
move = Vector3Normalize(move);

// Apply speed and gravity preservation
float speed = IsKeyDown(KEY_SHIFT) ? 50 : 10;
btVector3 velocity(move.x * speed, body->getLinearVelocity().y(), move.z * speed);
body->setLinearVelocity(velocity);
```

### Jump Implementation

**Single Impulse on Key Press**:
```cpp
if (IsKeyPressed(KEY_SPACE)) {
    body->applyCentralImpulse(btVector3(0, 5, 0));
}
// Note: Only if character is touching ground (todo: raycast check)
```


## Character System

### Both Player and SkeletonPlayer follow identical patterns:

#### Initialization
```cpp
Constructor(btDynamicsWorld* world, ...model paths..., Vector3 startPos)
  ├─ Load model/animation resources
  ├─ CreatePhysicsBody(startPos)
  └─ Initialize animation state
```

#### Physics Body Creation
```cpp
void CreatePhysicsBody(const Vector3& startPos) {
    // 1. Create collision shape
    shape = new btCapsuleShape(radius, height);

    // 2. Set initial transform
    btTransform startTransform;
    startTransform.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));

    // 3. Create rigid body
    motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(...);
    body = new btRigidBody(rbInfo);

    // 4. Configure constraints
    body->setAngularFactor(btVector3(0, 1, 0));  // Only Y rotation
    body->setActivationState(DISABLE_DEACTIVATION);  // Always active

    // 5. Add to world
    world->addRigidBody(body);
}
```

#### Update Loop
```cpp
void Update(float deltaTime) {
    // 1. Read input (WASD, Space, Shift)
    Vector3 move = {0, 0, 0};
    float speed = 10;  // or 50 if sprinting

    // 2. Apply forces to physics body
    body->setLinearVelocity(btVector3(move.x * speed, preserve_y, move.z * speed));

    // 3. Update rotation to face camera direction
    SyncRotation();  // Rotate body to match camera yaw

    // 4. Update animation state
    // (Different for Player vs SkeletonPlayer)
}
```

#### Rendering
```cpp
void Render() {
    // 1. Get physics body position
    btTransform trans;
    body->getMotionState()->getWorldTransform(trans);
    Vector3 position = ExtractPosition(trans);

    // 2. Offset to align model feet with ground
    position.y -= modelHeight * 0.5f;  // Varies by character type

    // 3. Draw model/skeleton at position
    DrawModel(model, position, 1.0f, WHITE);  // or
    renderer.DrawSkeletonTransformed(skeleton, matrices, position, ...);
}
```

### Differences

| Aspect | Player (Rick) | SkeletonPlayer |
|--------|-------|---------|
| Animation | Raylib ModelAnimation | OZZ Skeletal Animation |
| Loading | LoadModel(path) + LoadModelAnimations() | SkeletonAnimation.LoadSkeleton/Animation() |
| Playback | Frame counter `animFrameCounter++` | Time-based `current_time_` |
| Control | Always advancing frames | Play/Stop based on movement |
| Update | `UpdateModelAnimation(model, anim, frame)` | `anim.Update(dt)` |
| Render | `DrawModel(model, pos, ...)` | `renderer.DrawSkeletonTransformed(...)` |
