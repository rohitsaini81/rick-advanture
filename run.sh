#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
build_dir="$root_dir/build"

cache_file="$build_dir/CMakeCache.txt"

if [[ -f "$cache_file" ]]; then
  cache_root="$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$cache_file" | head -n 1)"
  if [[ -n "${cache_root:-}" && "$cache_root" != "$root_dir" ]]; then
    echo "Stale CMake cache detected; removing $build_dir"
    rm -rf "$build_dir"
  fi
fi

if [[ ! -f "$cache_file" ]]; then
  cmake -S "$root_dir" -B "$build_dir"
fi

cmake --build "$build_dir"

exec "$build_dir/ab_rick"
