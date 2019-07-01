#pragma once

// TODO: Refactor most of this, this is all very recycled code and needs to be cleaned up in the source

#include "common.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>

class Shader {
public:
	Shader();
	Shader(Shader&& other);
	Shader(std::string vert_src, std::string frag_src);
	~Shader();
	Shader& operator=(Shader&& other);
	std::optional<u32> handle();
	void destroy();
private:
	std::optional<u32> m_id = std::nullopt;
};