
#include "sprite_cache.h"
#include "std_filesystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "nlohmann/json.hpp"
#include "glad/glad.h"
#include "gl_err.h"
#include "glm/glm.hpp"

#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace {

	struct Sprite {
		//glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 uv = { 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec2 origin = { 0.0f, 0.0f };
		u32 image_index = 0;
		u32 pad;
	};
	std::unordered_map<std::string, u32> sprites;
	std::optional<u32> texture_array;
	std::optional<u32> sprite_buffer;

}

void SpriteCache::startup() {

	stbi_set_flip_vertically_on_load(true);
	struct Pixel {
		u32 vals[4];
	};
	std::vector<Pixel> array_texture_buffer;
	std::vector<Sprite> sprite_staging_buffer;
	u32 cur_image_index = 0;

	std::vector<unsigned char*> image_datas;

	for (auto& dir : std_filesystem::recursive_directory_iterator("data/atlas/")) { // God I love std::filesystem

		if (dir.path().extension().string() == ".json") {

			std::ifstream atlas_file;
			std::stringstream atlas_stream;
			atlas_file.open(dir.path());
			atlas_stream << atlas_file.rdbuf();
			atlas_file.close();
			json atlas = json::parse(atlas_stream.str());

			json atlas_meta = atlas["meta"];
			std::string atlas_image = atlas_meta["image"];

			u32 handle;

			b32 loaded = false;
			for (auto& dir : std_filesystem::recursive_directory_iterator("data/texture/")) {
				if (dir.path().filename() == atlas_image) {

					int x, y, c;
					unsigned char* data = stbi_load(dir.path().u8string().c_str(), &x, &y, &c, 0);
					if (data && x == 1024 && y == 1024) {
						//u32 old_size = array_texture_buffer.size();
						//u32 new_size = old_size + (1024 * 1024 * 4);
						//array_texture_buffer.resize(new_size);
						//memcpy(array_texture_buffer[old_size], data, (1024 * 1024 * 4));
						image_datas.push_back(data);
						loaded = true;
						break;
					}
					else if (data) {
						stbi_image_free(data);
						break;
					}

				}
			}

			if (loaded) {

				std::vector<json> sprite_frames = atlas["frames"];
				for (auto& sprite : sprite_frames) {
					std::string name = sprite["filename"];
					u32 x = sprite["frame"]["x"];
					u32 y = sprite["frame"]["y"];
					u32 w = sprite["frame"]["w"];
					u32 h = sprite["frame"]["h"];
					f32 origin_x = sprite["pivot"]["x"];
					f32 origin_y = sprite["pivot"]["y"];

					Sprite new_sprite;
					new_sprite.uv = { (f32)x, (f32)y, (f32)w, (f32)h };
					new_sprite.origin = { origin_x, origin_y };
					new_sprite.image_index = cur_image_index;

					sprite_staging_buffer.push_back(new_sprite);

					sprites[name] = { static_cast<u32>(sprite_staging_buffer.size() - 1) };

				}

				cur_image_index++;

			}

		}
	}

	u32 texture, buffer;

	glGenTextures(1, &texture); CheckGLError();
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 1024, 1024, image_datas.size()); CheckGLError();

	for (int i = 0; i < image_datas.size(); ++i) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 1024, 1024, 1, GL_RGBA, GL_UNSIGNED_BYTE, image_datas[i]); CheckGLError();
		stbi_image_free(image_datas[i]);
	}

	texture_array = texture;
	 
	glGenBuffers(1, &buffer); CheckGLError();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer); CheckGLError();
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Sprite) * sprite_staging_buffer.size(), sprite_staging_buffer.data(), GL_STATIC_READ); CheckGLError();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); CheckGLError();

	sprite_buffer = buffer;
}

void SpriteCache::shutdown() {

	if (texture_array.has_value()) {
		glDeleteTextures(1, &texture_array.value()); CheckGLError();
	}
	if (sprite_buffer.has_value()) {
		glDeleteBuffers(1, &sprite_buffer.value()); CheckGLError();
	}

}

std::optional<const u32> SpriteCache::getCacheTexture() {
	return texture_array;
}

std::optional<const u32> SpriteCache::getShaderSpriteBuffer() {
	return sprite_buffer;
}

std::optional<const u32> SpriteCache::fetchSprite(std::string name) {
	for (auto& sprite : sprites) {
		if (sprite.first == name) {
			return { sprite.second };
		}
	}
	return std::nullopt;
}