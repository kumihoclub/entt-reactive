#pragma once

#include "common.h"
#include "handle.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	Shader(Shader&& other);
	~Shader();
	void bind();
	i32 getUniform(std::string name);
	void setValue(i32 uniform, i32 val);
	void setValue(i32 uniform, u32 val);
	void setValue(i32 uniform, f32 val);
	void setValue(i32 uniform, glm::vec2 val);
	void setValue(i32 uniform, glm::vec3 val);
	void setValue(i32 uniform, glm::vec4 val);
	void setValue(i32 uniform, glm::mat4 val);
	static handle<Shader> create(std::string vert_src, std::string frag_src);
	static void destroy(handle<Shader>& handle);
	// Management class
	class Manager {
	public:
		void init();
		void shutdown();
	};
private:
	Shader(std::string vert_src, std::string frag_src);
	u32 m_id = 0;
};