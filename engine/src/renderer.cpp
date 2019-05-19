
#include "renderer.h"
#include "window.h"
#include "gl_err.h"

#include "glm/gtc/matrix_transform.hpp"

void Renderer::init() {

	m_renderable.staging_buffer.reserve(25000);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate/compile resources
	glGenBuffers(1, &m_renderable.shader_handle);
	glGenVertexArrays(1, &m_vao); CheckGLError();
	glGenFramebuffers(1, &m_framebuffer.handle); CheckGLError();
	glGenTextures(1, &m_framebuffer.texture); CheckGLError();

	std::string vert =
#include "shader/draw.vert"
		;
	std::string frag =
#include "shader/draw.frag"
		;
	m_renderable.shader = Shader::create(vert, frag);
	m_renderable.sprite_texture_uniform = 0;
	m_renderable.view_uniform = 1;
	m_renderable.proj_uniform = 2;
	m_renderable.offset_uniform = 3;

	vert =
#include "shader/filter.vert"
		;
	frag =
#include "shader/filter.frag"
		;
	m_filter.shader = Shader::create(vert, frag);

	// Setup FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.handle); CheckGLError();
	glBindTexture(GL_TEXTURE_2D, m_framebuffer.texture); CheckGLError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::View::getWidth(), Window::View::getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CheckGLError();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebuffer.texture, 0); CheckGLError();
	glBindTexture(GL_TEXTURE_2D, 0); CheckGLError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); CheckGLError();

	m_context = glfwGetCurrentContext();
	glfwMakeContextCurrent(NULL);

	m_running = true;

	m_thread = std::thread([&] {

		glfwMakeContextCurrent(m_context);
		while (m_running) {

			std::unique_lock<std::mutex> lock(m_condition_lock);
			m_condition_var.wait(lock, [&] {return (m_ready_to_draw == true); });
			if (!m_running) { glfwMakeContextCurrent(NULL); return 0; };
			//if (m_ready_to_draw) {
			m_ready_to_draw = false;
			m_is_drawing = true;
			m_frame->consumeDrawCommands(m_renderable.staging_buffer);


			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 proj = glm::mat4(1.0f);
			proj = glm::ortho(-((f32)Window::View::getWidth() / 2), ((f32)Window::View::getWidth() / 2), -((f32)Window::View::getHeight() / 2), ((f32)Window::View::getHeight() / 2), -64.0f, 64.0f);

			m_renderable.shader->bind(); 
			m_renderable.shader->setValue(m_renderable.offset_uniform, 0);
			m_renderable.shader->setValue(m_renderable.view_uniform, view);
			m_renderable.shader->setValue(m_renderable.proj_uniform, proj);
			m_renderable.shader->setValue(m_renderable.sprite_texture_uniform, 0);

			glBindVertexArray(m_vao);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderable.shader_handle);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_renderable.shader_handle); CheckGLError();
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.handle); CheckGLError();
			glActiveTexture(GL_TEXTURE0);
			glClear(GL_COLOR_BUFFER_BIT);

			i32 offset = 0;
			i32 range = 0;

			if (m_renderable.staging_buffer.size() > 0) {
				if (m_renderable.shader_buffer_capacity < m_renderable.staging_buffer.size()) {
					glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Renderable) * m_renderable.staging_buffer.size(), m_renderable.staging_buffer.data(), GL_DYNAMIC_DRAW); CheckGLError();
					m_renderable.shader_buffer_capacity = m_renderable.staging_buffer.size();
				}
				else {
					GLvoid* p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Renderable) * m_renderable.staging_buffer.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT); CheckGLError();
					if (p != NULL) {
						memcpy(p, m_renderable.staging_buffer.data(), sizeof(Renderable) * m_renderable.staging_buffer.size());
						glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); CheckGLError();
					}
				}
			}

			m_renderable.shader->setValue(m_renderable.offset_uniform, 0);
			for (u32 i = 0; i < m_renderable.staging_buffer.size(); ++i) {
				if (i != 0 && m_renderable.staging_buffer[i].texture_id != m_renderable.staging_buffer[i - 1].texture_id) {
					range = i;
					glActiveTexture(GL_TEXTURE0); CheckGLError();
					glBindTexture(GL_TEXTURE_2D, m_renderable.staging_buffer[i - 1].texture_id); CheckGLError();
					glDrawArrays(GL_TRIANGLES, 0, (range - offset) * 6); CheckGLError();
					offset = range;
					m_renderable.shader->setValue(m_renderable.offset_uniform, offset);
				}
			}
			if (m_renderable.staging_buffer.size() > 0) {
				range = m_renderable.staging_buffer.size();
				glActiveTexture(GL_TEXTURE0); CheckGLError();
				glBindTexture(GL_TEXTURE_2D, m_renderable.staging_buffer[m_renderable.staging_buffer.size() - 1].texture_id); CheckGLError();
				glDrawArrays(GL_TRIANGLES, 0, (range - offset) * 6); CheckGLError();
				m_renderable.shader->setValue(m_renderable.offset_uniform, 0);
			}

			m_filter.shader->bind();
			m_renderable.shader->setValue(m_filter.render_texture_uniform, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0); CheckGLError();
			glClear(GL_COLOR_BUFFER_BIT); CheckGLError();
			glActiveTexture(GL_TEXTURE0); CheckGLError();
			//glUniform1i(0, 0); CheckGLError();
			glBindTexture(GL_TEXTURE_2D, m_framebuffer.texture); CheckGLError();
			glDrawArrays(GL_TRIANGLES, 0, 6); CheckGLError();
			glfwSwapBuffers(m_context);
			m_is_drawing = false;

		}

		glfwMakeContextCurrent(NULL);
		return 0;
	});

}

b32 Renderer::is_drawing() {
	return m_is_drawing;
}

void Renderer::draw(Frame& frame) {

	if (!m_is_drawing) {
		m_frame = &frame;
		m_ready_to_draw = true;
		m_condition_var.notify_all();
	}

}

void Renderer::wait() {
	//while (m_is_drawing) {}
}

void Renderer::shutdown() {

	while (m_is_drawing) {}
	m_ready_to_draw = true; // Set this to true here to ensure it passes the condition var check if it is waiting
	m_running = false;
	m_condition_var.notify_all();
	m_thread.join();
	glfwMakeContextCurrent(m_context);

	glBindVertexArray(0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//u32 handles[2] = { m_renderable.shader_handle, m_vertex_dummies.vbo };
	glDeleteBuffers(1, &m_renderable.shader_handle);
	glDeleteFramebuffers(1, &m_framebuffer.handle);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteTextures(1, &m_framebuffer.texture);

}