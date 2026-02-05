#!/usr/bin/env bash
set -e

echo "=== Setting up externals ==="

clone_if_missing() {
    local repo=$1
    local dir=$2

    if [ -d "$dir" ]; then
        echo "[SKIP] $dir already exists"
    else
        echo "[CLONE] $repo → $dir"
        git clone --depth 1 "$repo" "$dir"
    fi
}

mkdir -p externals
cd externals

clone_if_missing "https://github.com/bulletphysics/bullet3.git" "bullet3"
clone_if_missing "https://github.com/ocornut/imgui.git" "imgui"
clone_if_missing "https://github.com/lua/lua.git" "lua-5.1.5"
clone_if_missing "https://github.com/raysan5/raylib.git" "raylib"
clone_if_missing "https://github.com/RobLoach/raylib-cpp.git" "raylib-cpp"
clone_if_missing "https://github.com/raylib-extras/rlImGui.git" "rlImGui"
clone_if_missing "https://github.com/guillaumeblanc/ozz-animation.git" "ozz-animation"
echo "=== Done! ==="

