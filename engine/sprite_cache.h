#pragma once

#include "common.h"

#include <optional>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

class SpriteCache {
public:
	void startup();
	void shutdown();
	static std::optional<const u32> fetchSprite(std::string name);
	std::optional<const u32> getCacheTexture();
	std::optional<const u32> getShaderSpriteBuffer();
};

/*namespace Sprite {

	class Cache {
	public:
		void startup();
		void shutdown();
	};

	std::optional<const u32> fetchSprite(std::string name);
	std::optional<const u32> getCacheTexture();
	std::optional<const u32> getCacheBuffer();

}*/