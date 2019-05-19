#pragma once

#include "common.h"
#include "borrow.h"
#include "sprite.h"
#include "shader.h"
#include "texture.h"

#include "glm/glm.hpp"

struct Material {
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Sprite sprite;
	b32 flip_x = false;
	b32 flip_y = false;
};