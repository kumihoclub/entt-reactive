#pragma once

#include "common.h"

#include "glm/glm.hpp"

#include <string>

struct Config {
	std::string title = "Grimoire";
	glm::uvec2 window_size = { 960, 540 };
	u32 sprite_renderable_capacity = 5000;
	u32 unit_pixels = 32;
	static void set(Config& config);
	static const Config& get();
};