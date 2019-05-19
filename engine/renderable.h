#pragma once

#include "common.h"
#include "glm/glm.hpp"

struct Renderable {
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 uv[6];
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	f32 glitch = 0.0f;
	// Struct has to be padded by and extra 3 32 values for shader alignment, just go ahead and use one for the texture ID for deciding batch strides.
	u32 texture_id = 0;
	glm::uvec2 padding;
};