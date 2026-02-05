#include "skeleton_renderer.h"
#include <ozz/base/maths/simd_math.h>
#include <cmath>

SkeletonRenderer::SkeletonRenderer() {
}

SkeletonRenderer::~SkeletonRenderer() {
}

Vector3 SkeletonRenderer::ExtractPosition(const ozz::math::Float4x4& matrix) {
    return {
        ozz::math::GetX(matrix.cols[3]),
        ozz::math::GetY(matrix.cols[3]),
        ozz::math::GetZ(matrix.cols[3])
    };
}

float SkeletonRenderer::ExtractScale(const ozz::math::Float4x4& matrix) {
    // Calculate scale from the basis vectors
    float x_val = ozz::math::GetX(matrix.cols[0]);
    float y_val = ozz::math::GetY(matrix.cols[0]);
    float z_val = ozz::math::GetZ(matrix.cols[0]);
    float sx = std::sqrt(x_val * x_val + y_val * y_val + z_val * z_val);
    return sx;
}

void SkeletonRenderer::DrawSkeleton(
    const ozz::animation::Skeleton& skeleton,
    const std::vector<ozz::math::Float4x4>& model_matrices,
    Color joint_color,
    Color bone_color,
    float joint_size) {

    const auto& parents = skeleton.joint_parents();

    for (int i = 0; i < skeleton.num_joints(); i++) {
        Vector3 pos = ExtractPosition(model_matrices[i]);

        // Draw joint sphere
        DrawSphere(pos, joint_size, joint_color);

        // Draw bone to parent
        int parent_idx = parents[i];
        if (parent_idx >= 0) {
            Vector3 parent_pos = ExtractPosition(model_matrices[parent_idx]);
            DrawLine3D(parent_pos, pos, bone_color);
        }
    }
}

void SkeletonRenderer::DrawSkeletonTransformed(
    const ozz::animation::Skeleton& skeleton,
    const std::vector<ozz::math::Float4x4>& model_matrices,
    Vector3 position,
    Quaternion rotation,
    float scale,
    Color joint_color,
    Color bone_color) {

    const auto& parents = skeleton.joint_parents();

    for (int i = 0; i < skeleton.num_joints(); i++) {
        Vector3 local_pos = ExtractPosition(model_matrices[i]);

        // Apply scale
        local_pos.x *= scale;
        local_pos.y *= scale;
        local_pos.z *= scale;

        // Apply position offset
        Vector3 final_pos = {
            local_pos.x + position.x,
            local_pos.y + position.y,
            local_pos.z + position.z
        };

        // Draw joint sphere
        DrawSphere(final_pos, scale * 0.03f, joint_color);

        // Draw bone to parent
        int parent_idx = parents[i];
        if (parent_idx >= 0) {
            Vector3 parent_local_pos = ExtractPosition(model_matrices[parent_idx]);
            parent_local_pos.x *= scale;
            parent_local_pos.y *= scale;
            parent_local_pos.z *= scale;

            Vector3 parent_final_pos = {
                parent_local_pos.x + position.x,
                parent_local_pos.y + position.y,
                parent_local_pos.z + position.z
            };

            DrawLine3D(parent_final_pos, final_pos, bone_color);
        }
    }
}
