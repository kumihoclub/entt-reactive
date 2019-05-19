#pragma once

#include "common.h"
#include "handle.h"

#include "glm/glm.hpp"

#include <optional>
#include <string>
// Self managed

//class TextureHandle;

class Texture  {
public:
	glm::uvec2 size;
	u32 num_channels;
	u32 id;
	static std::optional<Texture> create(std::string p_name, u32 p_x, u32 p_y, u32 p_c, void* data);
	static std::optional<Texture> fetch(std::string p_name);
	static void destroy(Texture texture);
	// Management class
	class Manager {
	public:
		void init();
		void shutdown();
	};
};