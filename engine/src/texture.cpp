
#include "glad/glad.h"
#include "gl_err.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "std_filesystem.h"

#include <unordered_map>
#include <vector>

// Static data
namespace {

	std::unordered_map<std::string, Texture> cache;

}

std::optional<Texture> Texture::create(std::string p_name, u32 p_x, u32 p_y, u32 p_c, void* data) {

	u32 new_id;
	glGenTextures(1, &new_id);
	glBindTexture(GL_TEXTURE_2D, new_id); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CheckGLError();
	if (p_c == 4) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); CheckGLError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, p_x, p_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); CheckGLError();
	}
	else if (p_c == 3) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CheckGLError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_x, p_y, 0, GL_RGB, GL_UNSIGNED_BYTE, data); CheckGLError();
	}
	else if (p_c == 2) { // when would this even be used?
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CheckGLError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, p_x, p_y, 0, GL_RG, GL_UNSIGNED_BYTE, data); CheckGLError();
	}
	else if (p_c == 1) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CheckGLError();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, p_x, p_y, 0, GL_RED, GL_UNSIGNED_BYTE, data); CheckGLError();
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	Texture new_texture{ glm::vec2{p_x, p_y}, p_c, new_id };
	cache[p_name] = new_texture;
	return cache[p_name];
}

std::optional<Texture> Texture::fetch(std::string p_name) {
	if (cache.find(p_name) != cache.end()) {
		return cache[p_name];
	}
	return std::nullopt;
}

void Texture::destroy(Texture texture) {
	for (auto it = cache.begin(); it != cache.end(); ++it) {
		if (it->second.id == texture.id) {
			glDeleteTextures(1, &texture.id);
			cache.erase(it);
			break;
		}
	}
}

void Texture::Manager::init() {
	stbi_set_flip_vertically_on_load(true);
	for (auto& dir : std_filesystem::recursive_directory_iterator("data/texture/")) { // God I love std::filesystem
		if (dir.path().extension().string() == ".png") {

			int x, y, c;
			unsigned char* data = stbi_load(dir.path().u8string().c_str(), &x, &y, &c, 0);
			if (data) {
				// Remove the "data" suffix from the directory
				std::string parsed_path = dir.path().u8string();
				std::string data_rem = "data\\";
				std::string::size_type i = parsed_path.find(data_rem);
				if (i != std::string::npos) { parsed_path.erase(i, data_rem.length()); }
				Texture::create(parsed_path, x, y, c, data);
				stbi_image_free(data);
			}

		}
	}
	glActiveTexture(GL_TEXTURE0);
}

void Texture::Manager::shutdown() {
	cache.clear();
}