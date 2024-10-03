#pragma once

#include "imgui.h"

#include "camera.hpp"
#include "renderer.hpp"

namespace Globals {

inline Camera camera;
inline Renderer* renderer;
inline float delta_time = 0.0f;
inline constexpr float time_step = 0.01f;

}

