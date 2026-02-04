#include "raylib.h"
#include <ozz/base/maths/float4x4.h>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/local_to_model_job.h>

#include <ozz/base/io/archive.h>
#include <ozz/base/io/stream.h>

#include <vector>
#include <cmath>
#include <cstdio>




#include "3dObjects/objects.h"
#include "Controls/camera.h"
#include "ETC/global_var.h"
// ------------------------------------------------------------
// Load OZZ Skeleton
// ------------------------------------------------------------
bool LoadSkeleton(const char* path, ozz::animation::Skeleton& skeleton)
{
    ozz::io::File file(path, "rb");
    if (!file.opened())
    {
        printf("Failed to open skeleton: %s\n", path);
        return false;
    }

    ozz::io::IArchive archive(&file);
    archive >> skeleton;

    return true;
}

// ------------------------------------------------------------
// Load OZZ Animation
// ------------------------------------------------------------
bool LoadAnimation(const char* path, ozz::animation::Animation& anim)
{
    ozz::io::File file(path, "rb");
    if (!file.opened())
    {
        printf("Failed to open animation: %s\n", path);
        return false;
    }

    ozz::io::IArchive archive(&file);
    archive >> anim;

    return true;
}

// ------------------------------------------------------------
// Extract translation from matrix
// ------------------------------------------------------------
Vector3 GetPosition(const ozz::math::Float4x4& m)
{
    return {
        m.cols[3].x,
        m.cols[3].y,
        m.cols[3].z
    };
}

// ------------------------------------------------------------
// Draw skeleton using raylib
// ------------------------------------------------------------
void DrawSkeleton(
    const ozz::animation::Skeleton& skel,
    const std::vector<ozz::math::Float4x4>& models)
{
    const auto& parents = skel.joint_parents();

    for (int i = 0; i < skel.num_joints(); i++)
    {
        int parent = parents[i];
        if (parent < 0) continue;

        Vector3 p = GetPosition(models[i]);
        Vector3 pp = GetPosition(models[parent]);

        DrawLine3D(pp, p, RED);
        DrawSphere(p, 0.02f, BLUE);
    }
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main()
{
        std::string modelPath2 = project_dir + "/assets/rick/rick.glb";

    const int screenWidth = 1280;
    const int screenHeight = 800;

    raylib::Window window(
        screenWidth,
        screenHeight,
        "raylib + ozz-animation Test"
    );

    SetTargetFPS(60);

    // ---------------- Camera ----------------
    raylib::Camera3D camera;
    camera.position = { 2.0f, 2.0f, 4.0f };
    camera.target = { 0.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // ---------------- Load OZZ ----------------

    ozz::animation::Skeleton skeleton;
    ozz::animation::Animation animation;

    if (!LoadSkeleton(
            (project_dir + "/assets/ozz/animation.ozz").c_str(),
    animation
    ))
        return 1;

    if (!LoadAnimation(
        
            (project_dir + "/assets/ozz/animation.ozz").c_str(),
    animation)
    )
        return 1;

    printf("Skeleton joints: %d\n", skeleton.num_joints());
    printf("Animation duration: %.2f\n", animation.duration());

    // ---------------- Buffers ----------------

    std::vector<ozz::math::SoaTransform> locals;
    std::vector<ozz::math::Float4x4> models;

    locals.resize(skeleton.num_soa_joints());
    models.resize(skeleton.num_joints());

    // Sampling context
    ozz::animation::SamplingJob::Context context;
    context.Resize(skeleton.num_joints());

    // Jobs
    ozz::animation::SamplingJob sampling;
    ozz::animation::LocalToModelJob ltm;

    sampling.animation = &animation;
    sampling.context = &context;
    sampling.output = make_span(locals);

    ltm.skeleton = &skeleton;
    ltm.input = make_span(locals);
    ltm.output = make_span(models);

    // ---------------- Time ----------------

    float time = 0.0f;

    // ---------------- Main Loop ----------------

    while (!window.ShouldClose())
    {
        float dt = GetFrameTime();
        time += dt;

        // Loop animation
        float ratio = fmod(time, animation.duration())
                      / animation.duration();

        // Sample animation
        sampling.ratio = ratio;
        sampling.Run();

        // Local -> Model
        ltm.Run();

        // Camera orbit
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 delta = GetMouseDelta();

            camera.position.x += delta.x * 0.01f;
            camera.position.z += delta.y * 0.01f;
        }

        // ---------------- Render ----------------

        BeginDrawing();

        ClearBackground({ 30, 30, 35, 255 });

        BeginMode3D(camera);

        DrawGrid(20, 0.5f);

        DrawSkeleton(skeleton, models);

        EndMode3D();

        DrawText("OZZ + raylib test", 20, 20, 20, GREEN);
        DrawText("Right mouse: rotate camera", 20, 45, 16, GRAY);

        EndDrawing();
    }

    return 0;
}

