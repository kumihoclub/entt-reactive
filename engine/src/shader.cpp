
#include "shader.h"

#include "gl_err.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader() {}

Shader::Shader(Shader&& other) {
	m_id = other.m_id;
	other.m_id = std::nullopt;
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
			program_id = glCreateProgram(); CheckGLError();
			glAttachShader(program_id, vertex); CheckGLError();
			glAttachShader(program_id, fragment); CheckGLError();
			glLinkProgram(program_id); CheckGLError();
			// print linking errors if any
			glGetProgramiv(program_id, GL_LINK_STATUS, &success); CheckGLError();
			if (!success) {
				glGetProgramInfoLog(program_id, 512, NULL, infoLog);
				std::cout << "ERROR: Shader program creation failed, skipping shader creation." << infoLog << std::endl;
				program_id = 0;
				assert(success);
			}

		}
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex); CheckGLError();
	glDeleteShader(fragment); CheckGLError();

	if (success) {
		m_id = program_id;
	}
}

Shader::~Shader() {
	destroy();
}

Shader& Shader::operator=(Shader&& other) {
	m_id = other.m_id;
	other.m_id = std::nullopt;
	return *this;
}

std::optional<u32> Shader::handle() {
	return m_id;
}

void Shader::destroy() {
	if (m_id.has_value()) {
		glDeleteProgram(m_id.value()); CheckGLError();
		m_id = std::nullopt;
	}
}
