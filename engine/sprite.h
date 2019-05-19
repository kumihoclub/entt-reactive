#pragma once

#include "common.h"
#include "texture.h"

#include "glm/glm.hpp"

#include <optional>

#define SPRITE_ORIGIN_CENTER glm::vec2(0.0f, 0.0f);
#define SPRITE_ORIGIN_TOP_LEFT glm::vec2(-1.0f, -1.0f);
#define SPRITE_ORIGIN_TOP_RIGHT glm::vec2(1.0f, -1.0f);
#define SPRITE_ORIGIN_BOTTOM_LEFT glm::vec2(-1.0f, 1.0f);
#define SPRITE_ORIGIN_BOTTOM_RIGHT glm::vec2(1.0f, 1.0f);

struct Sprite {
	Texture texture;
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 origin = SPRITE_ORIGIN_CENTER;
	glm::uvec2 size = { 0.0f, 0.0f };
	glm::uvec2 uv = { 0.0f, 0.0f };
	f32 glitch = 0.0f;
};