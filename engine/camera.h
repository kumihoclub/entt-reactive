#pragma once

#include "common.h"

#include "glm/glm.hpp"

struct Camera {
	glm::vec2 pos = { 0.0f, 0.0f };
	f32 zoom = 1.0f;
};