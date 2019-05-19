#pragma once

#include "common.h"

#include "glm/glm.hpp"

struct Transform {
	glm::vec2 pos = { 0.0f, 0.0f };
	glm::vec2 scale = { 1.0f, 1.0f };
	glm::vec2 vel = { 0.0f, 0.0f };
	u32 layer = 0;
	f32 rot = 0.0f;
};