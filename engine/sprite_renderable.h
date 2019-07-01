#pragma once

#include "common.h"
#include "frame.h"

#include "glm/glm.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <atomic>

struct SpriteRenderable {
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	u32 sprite_index = 0;
	f32 glitch = 0.0f;
	glm::uvec2 pad;
};