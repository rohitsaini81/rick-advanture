#include "skeleton_animation.h"
#include <ozz/base/io/archive.h>
#include <ozz/base/io/stream.h>
#include <cmath>
#include <cstdio>

SkeletonAnimation::SkeletonAnimation()
    : current_time_(0.0f),
      playback_speed_(1.0f),
      is_playing_(false),
      is_looping_(true) {
}

SkeletonAnimation::~SkeletonAnimation() {
}

bool SkeletonAnimation::LoadSkeleton(const char* path) {
    ozz::io::File file(path, "rb");
    if (!file.opened()) {
        printf("Failed to open skeleton: %s\n", path);
        return false;
    }

    ozz::io::IArchive archive(&file);
    archive >> skeleton_;

    printf("Loaded skeleton with %d joints\n", skeleton_.num_joints());

    // Resize buffers
    local_transforms_.resize(skeleton_.num_soa_joints());
    model_matrices_.resize(skeleton_.num_joints());

    // Setup sampling context
    sampling_context_.Resize(skeleton_.num_joints());

    // Setup sampling job
    // sampling_job_.animation = nullptr;
    sampling_job_.context = &sampling_context_;
    sampling_job_.output = ozz::make_span(local_transforms_);

    // Setup local-to-model job
    ltm_job_.skeleton = &skeleton_;
    ltm_job_.input = ozz::make_span(local_transforms_);
    ltm_job_.output = ozz::make_span(model_matrices_);

    return true;
}


bool SkeletonAnimation::LoadAnimation(const std::string& name, const char* path) {
    ozz::animation::Animation anim;

    ozz::io::File file(path, "rb");
    if (!file.opened()) {
        printf("Failed to open animation: %s\n", path);
        return false;
    }

    ozz::io::IArchive archive(&file);
    archive >> anim;

    animations_[name] = std::move(anim);

    printf("Loaded animation '%s'\n", name.c_str());
    return true;
}


void SkeletonAnimation::SetAnimation(const std::string& name) {
    auto it = animations_.find(name);
    if (it == animations_.end()) return;

    current_animation_ = name;
    sampling_job_.animation = &it->second;

    current_time_ = 0.0f;
}



void SkeletonAnimation::Update(float dt) {
    const auto* anim = GetCurrentAnimation();
    if (!is_playing_ || !anim || anim->duration() <= 0.0f){
        return;
    }

    current_time_ += dt * playback_speed_;

    // Handle looping
    if (is_looping_) {
        float ratio = fmod(current_time_, anim->duration()) / anim->duration();
        sampling_job_.ratio = ratio;
    } else {
        if (current_time_ >= anim->duration()) {
            current_time_ = anim->duration();
            is_playing_ = false;
        }
        sampling_job_.ratio = current_time_ / anim->duration();
    }

    // Sample animation
    sampling_job_.Run();

    // Transform local to model space
    ltm_job_.Run();
}

void SkeletonAnimation::SetPlaybackSpeed(float speed) {
    playback_speed_ = speed;
}

void SkeletonAnimation::SetLooping(bool loop) {
    is_looping_ = loop;
}

void SkeletonAnimation::Play() {
    is_playing_ = true;
}

void SkeletonAnimation::Stop() {
    is_playing_ = false;
}

void SkeletonAnimation::Reset() {
    current_time_ = 0.0f;
    is_playing_ = false;
}

void SkeletonAnimation::SetAnimationTime(float time) {
    const auto* anim = GetCurrentAnimation();

    if (anim->duration() <= 0.0f) {
        return;
    }

    // Clamp time to animation duration
    current_time_ = time;
    if (current_time_ > anim->duration()) {
        current_time_ = anim->duration();
    }
    if (current_time_ < 0.0f) {
        current_time_ = 0.0f;
    }

    // Sample at this time
    float ratio = current_time_ / anim->duration();
    sampling_job_.ratio = ratio;
    sampling_job_.Run();
    ltm_job_.Run();
}
