#ifndef SKELETON_ANIMATION_H
#define SKELETON_ANIMATION_H

#include <ozz/base/maths/simd_math.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <vector>
#include <unordered_map>
#include <string>

std::unordered_map<std::string, ozz::animation::Animation> animations_;
std::string current_animation_;

class SkeletonAnimation {
public:
    SkeletonAnimation();
    ~SkeletonAnimation();

    // Load skeleton and animation files
    bool LoadSkeleton(const char* path);
    bool LoadAnimation(const char* path);

    // Update animation playback
    void Update(float dt);
    void SetPlaybackSpeed(float speed);
    void SetLooping(bool loop);
    void Play();
    void Stop();
    void Reset();
    void SetAnimationTime(float time);

    // Getters
    const ozz::animation::Skeleton& GetSkeleton() const { return skeleton_; }
    const std::vector<ozz::math::Float4x4>& GetModelMatrices() const { return model_matrices_; }
    float GetCurrentTime() const { return current_time_; }
    float GetDuration() const { return animation_.duration(); }
    float GetPlaybackSpeed() const { return playback_speed_; }
    bool IsPlaying() const { return is_playing_; }

private:
    // OZZ data
    ozz::animation::Skeleton skeleton_;
    ozz::animation::Animation animation_;

    // Buffers
    std::vector<ozz::math::SoaTransform> local_transforms_;
    std::vector<ozz::math::Float4x4> model_matrices_;

    // Sampling context and jobs
    ozz::animation::SamplingJob::Context sampling_context_;
    ozz::animation::SamplingJob sampling_job_;
    ozz::animation::LocalToModelJob ltm_job_;

    // Playback state
    float current_time_;
    float playback_speed_;
    bool is_playing_;
    bool is_looping_;
};

#endif // SKELETON_ANIMATION_H
