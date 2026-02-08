#ifndef SKELETON_RENDERER_H
#define SKELETON_RENDERER_H

#include "raylib.h"
#include <ozz/base/maths/simd_math.h>
#include <ozz/animation/runtime/skeleton.h>
#include <vector>

class SkeletonRenderer {
public:
    SkeletonRenderer();
    ~SkeletonRenderer();

    // Draw skeleton with model matrices
    void DrawSkeleton(
        const ozz::animation::Skeleton& skeleton,
        const std::vector<ozz::math::Float4x4>& model_matrices,
        Color joint_color = BLUE,
        Color bone_color = RED,
        float joint_size = 0.03f);

    // Draw skeleton with rotation and position offset
    void DrawSkeletonTransformed(
        const ozz::animation::Skeleton& skeleton,
        const std::vector<ozz::math::Float4x4>& model_matrices,
        Vector3 position,
        Quaternion rotation,
        float scale = 1.0f,
        Color joint_color = BLUE,
        Color bone_color = RED);

    // Helper: Extract position from matrix
    static Vector3 ExtractPosition(const ozz::math::Float4x4& matrix);

    // Helper: Extract scale from matrix
    static float ExtractScale(const ozz::math::Float4x4& matrix);
};

#endif // SKELETON_RENDERER_H
