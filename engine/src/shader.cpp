
#include "shader.h"

#include "gl_err.h"

#include <glm/gtc/type_ptr.hpp>

// Static data
namespace {

	plf::colony<Shader> cache;
	u32 last_bound_id = 0; // Hacky, but fuck it

}

Shader::Shader(Shader&& other) {
	m_id = other.m_id;
	other.m_id = 0;
}

Shader::Shader(std::string vert_src, std::string frag_src) {
	const char* vert_src_c_str = vert_src.c_str();
	const char* frag_src_c_str = frag_src.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	u32 program_id = 0;

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER); CheckGLError();
	glShaderSource(vertex, 1, &vert_src_c_str, NULL); CheckGLError();
	glCompileShader(vertex); CheckGLError();
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); CheckGLError();
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR: Vertex shader compilation failed, skipping shader creation." << infoLog << std::endl;
		assert(success);
	}
	else {
		fragment = glCreateShader(GL_FRAGMENT_SHADER); CheckGLError();
		glShaderSource(fragment, 1, &frag_src_c_str, NULL); CheckGLError();
		glCompileShader(fragment); CheckGLError();
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); CheckGLError();
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR: Fragment shader compilation failed, skipping shader creation." << infoLog << std::endl;
			assert(!success);
		}
		else {
			// shader Program
			m_id = glCreateProgram(); CheckGLError();
			glAttachShader(m_id, vertex); CheckGLError();
			glAttachShader(m_id, fragment); CheckGLError();
			glLinkProgram(m_id); CheckGLError();
			// print linking errors if any
			glGetProgramiv(m_id, GL_LINK_STATUS, &success); CheckGLError();
			if (!success) {
				glGetProgramInfoLog(m_id, 512, NULL, infoLog);
				std::cout << "ERROR: Shader program creation failed, skipping shader creation." << infoLog << std::endl;
				m_id = 0;
				assert(success);
			}

		}
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex); CheckGLError();
	glDeleteShader(fragment); CheckGLError();

	/*if (success) {
		SPShader new_shader(program_id);
		shader_cache[name] = new_shader;
	}*/
}

Shader::~Shader() {
	if (m_id != 0) {
		glDeleteProgram(m_id); CheckGLError();
	}
}

void Shader::bind() {
	glUseProgram(m_id); CheckGLError();
}

i32 Shader::getUniform(std::string name) {
	return glGetUniformLocation(m_id, name.c_str()); CheckGLError();
}

void Shader::setValue(i32 uniform, u32 val) {
	glUniform1ui(uniform, val); CheckGLError();
}

void Shader::setValue(i32 uniform, i32 val) {
	glUniform1i(uniform, val); CheckGLError();
}

void Shader::setValue(i32 uniform, f32 val) { 
	glUniform1f(uniform, val); CheckGLError();
}

void Shader::setValue(i32 uniform, glm::vec2 val) {
	glUniform2f(uniform, val.x, val.y); CheckGLError();
}

void Shader::setValue(i32 uniform, glm::vec3 val) {
	glUniform3f(uniform, val.x, val.y, val.z); CheckGLError();
}

void Shader::setValue(i32 uniform, glm::vec4 val) {
	glUniform4f(uniform, val.x, val.y, val.z, val.w); CheckGLError();
}

void Shader::setValue(i32 uniform, glm::mat4 val) {
	glUniformMatrix4fv(uniform, 1, FALSE, glm::value_ptr(val));
}

handle<Shader> Shader::create(std::string vert_src, std::string frag_src) {
	Shader new_shader(vert_src, frag_src);
	plf::colony<Shader>::iterator new_it = cache.insert(std::move(new_shader));
	return handle<Shader>(new_it);
}

void Shader::destroy(handle<Shader>& handle) {
	if (*handle != nullptr) {
		cache.erase(*handle.it());
		handle = {};
	}
}

void Shader::Manager::init() {
	// Just here for uniformities sakes
}

void Shader::Manager::shutdown() {
	cache.clear();
}
