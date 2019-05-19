#pragma once

#include "common.h"
#include "glm/glm.hpp"

struct Renderable {
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec4 uv = { 0.0f, 0.0f, 1.0f, 1.0f }; // { top_x, top_y, size_x, size_y }
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	f32 glitch = 0.0f;
	// Everything below this is just treated as padding in shaders, can be used CPU side for various uses but must be 3 32bit values
	u32 texture_id = 0;
	glm::uvec2 padding;
};