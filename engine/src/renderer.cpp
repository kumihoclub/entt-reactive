
#include "renderer.h"
#include "window.h"
#include "gl_err.h"

#include "glm/gtc/matrix_transform.hpp"

void Renderer::init() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Allocate buffers and etc
	u32 buffers[2];
	glGenBuffers(2, buffers); CheckGLError();
	m_vertex_dummies.vbo = buffers[0];
	m_renderable_data.shader_handle = buffers[1];
	glGenVertexArrays(1, &m_vertex_dummies.vao); CheckGLError();
	glGenFramebuffers(1, &m_framebuffer.handle); CheckGLError();
	glGenTextures(1, &m_framebuffer.texture); CheckGLError();

	std::string vert =
#include "shader/draw.vert"
		;
	std::string frag =
#include "shader/draw.frag"
		;
	m_renderable_draw_params.shader = Shader::create(vert, frag);
	m_renderable_draw_params.sprite_texture_uniform = 0;
	m_renderable_draw_params.view_uniform = 1;
	m_renderable_draw_params.proj_uniform = 2;
	m_renderable_draw_params.offset_uniform = 3;

	vert =
#include "shader/filter.vert"
		;
	frag =
#include "shader/filter.frag"
		;
	m_filter_draw_params.shader = Shader::create(vert, frag);

	// Setup vertex dummies
	glBindVertexArray(m_vertex_dummies.vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_dummies.vbo); CheckGLError();
	f32 dummy;
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32), NULL, GL_STATIC_DRAW); CheckGLError();
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(f32), (void*)(0)); CheckGLError();
	// Keep both vbo and vao bound, we will never change them

	// Setup FBO

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.handle); CheckGLError();
	glBindTexture(GL_TEXTURE_2D, m_framebuffer.texture); CheckGLError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::View::getWidth(), Window::View::getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CheckGLError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CheckGLError();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebuffer.texture, 0); CheckGLError();
	glBindTexture(GL_TEXTURE_2D, 0); CheckGLError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0); CheckGLError();

}

void Renderer::draw(Frame& frame) {

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::ortho(-((f32)Window::View::getWidth() / 2), ((f32)Window::View::getWidth() / 2), -((f32)Window::View::getHeight() / 2), ((f32)Window::View::getHeight() / 2), -64.0f, 64.0f);

	m_renderable_draw_params.shader->bind();

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.handle); CheckGLError();
	glActiveTexture(GL_TEXTURE0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderable_data.shader_handle); CheckGLError();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_renderable_data.shader_handle); CheckGLError();
	/*frame.consumeBackgroundDrawCommands(m_renderable_data.staging_buffer);
	if (m_renderable_data.staging_buffer.size() > 0) {
		if (m_renderable_data.shader_buffer_capacity < m_renderable_data.staging_buffer.size()) {
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Renderable) * m_renderable_data.staging_buffer.size(), m_renderable_data.staging_buffer.data(), GL_DYNAMIC_COPY); CheckGLError();
			m_renderable_data.shader_buffer_capacity = m_renderable_data.staging_buffer.size();
		}
		else {
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); CheckGLError();
			if (p != NULL) {
				memcpy(p, m_renderable_data.staging_buffer.data(), sizeof(Renderable) * m_renderable_data.staging_buffer.size());
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); CheckGLError();
			}
		}
	}*/

	m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, 0);
	m_renderable_draw_params.shader->setValue(m_renderable_draw_params.view_uniform, view);
	m_renderable_draw_params.shader->setValue(m_renderable_draw_params.proj_uniform, proj);
	m_renderable_draw_params.shader->setValue(m_renderable_draw_params.sprite_texture_uniform, 0);

	i32 offset = 0;
	i32 range = 0;

	/*for (u32 i = 0; i < m_renderable_data.staging_buffer.size(); ++i) {
		if (i != 0 && m_renderable_data.staging_buffer[i].texture_id != m_renderable_data.staging_buffer[i - 1].texture_id) {
			range = i;
			glActiveTexture(GL_TEXTURE0); CheckGLError();
			glBindTexture(GL_TEXTURE_2D, m_renderable_data.staging_buffer[i - 1].texture_id); CheckGLError();
			glDrawArrays(GL_TRIANGLES, 0, range - offset); CheckGLError();
			offset = range;
			m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, offset);
			//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.view_uniform, view);
			//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.proj_uniform, proj);
			//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.sprite_texture_uniform, 0);
		}
	}
	if (m_renderable_data.staging_buffer.size() > 0) {
		range = m_renderable_data.staging_buffer.size();
		glActiveTexture(GL_TEXTURE0); CheckGLError();
		glBindTexture(GL_TEXTURE_2D, m_renderable_data.staging_buffer[m_renderable_data.staging_buffer.size() - 1].texture_id); CheckGLError();
		glDrawArrays(GL_TRIANGLES, 0, range - offset); CheckGLError();
		m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, 0);
		//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.view_uniform, view);
		//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.proj_uniform, proj);
		//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.sprite_texture_uniform, 0);
	}*/

	// TODO FILTER

	frame.consumeWorldDrawCommands(m_renderable_data.staging_buffer);
	if (m_renderable_data.staging_buffer.size() > 0) {
		if (m_renderable_data.shader_buffer_capacity < m_renderable_data.staging_buffer.size()) {
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Renderable) * m_renderable_data.staging_buffer.size(), m_renderable_data.staging_buffer.data(), GL_DYNAMIC_COPY); CheckGLError();
			m_renderable_data.shader_buffer_capacity = m_renderable_data.staging_buffer.size();
		}
		else {
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); CheckGLError();
			if (p != NULL) {
				memcpy(p, m_renderable_data.staging_buffer.data(), sizeof(Renderable) * m_renderable_data.staging_buffer.size());
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); CheckGLError();
			}
		}
	}

	offset = 0;
	range = 0;
	m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, 0);
	for (u32 i = 0; i < m_renderable_data.staging_buffer.size(); ++i) {
		if (i != 0 && m_renderable_data.staging_buffer[i].texture_id != m_renderable_data.staging_buffer[i - 1].texture_id) {
			range = i;
			glActiveTexture(GL_TEXTURE0); CheckGLError();
			glBindTexture(GL_TEXTURE_2D, m_renderable_data.staging_buffer[i - 1].texture_id); CheckGLError();
			glDrawArrays(GL_TRIANGLES, 0, range - offset); CheckGLError();
			offset = range;
			m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, offset);
		}
	}
	if (m_renderable_data.staging_buffer.size() > 0) {
		range = m_renderable_data.staging_buffer.size();
		glActiveTexture(GL_TEXTURE0); CheckGLError();
		glBindTexture(GL_TEXTURE_2D, m_renderable_data.staging_buffer[m_renderable_data.staging_buffer.size() - 1].texture_id); CheckGLError();
		glDrawArrays(GL_TRIANGLES, 0, range - offset); CheckGLError();
		m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, 0);
	}

	// TODO FILTER

	/*frame.consumeGuiDrawCommands(m_renderable_data.staging_buffer);
	if (m_renderable_data.staging_buffer.size() > 0) {
		if (m_renderable_data.shader_buffer_capacity < m_renderable_data.staging_buffer.size()) {
			glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Renderable) * m_renderable_data.staging_buffer.size(), m_renderable_data.staging_buffer.data(), GL_DYNAMIC_COPY); CheckGLError();
			m_renderable_data.shader_buffer_capacity = m_renderable_data.staging_buffer.size();
		}
		else {
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); CheckGLError();
			if (p != NULL) {
				memcpy(p, m_renderable_data.staging_buffer.data(), sizeof(Renderable) * m_renderable_data.staging_buffer.size());
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); CheckGLError();
			}
		}
	}

	offset = 0;
	range = 0;
	m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, 0);
	for (u32 i = 0; i < m_renderable_data.staging_buffer.size(); ++i) {
		if (i != 0 && m_renderable_data.staging_buffer[i].texture_id != m_renderable_data.staging_buffer[i - 1].texture_id) {
			range = i;
			glActiveTexture(GL_TEXTURE0); CheckGLError();
			glBindTexture(GL_TEXTURE_2D, m_renderable_data.staging_buffer[i - 1].texture_id); CheckGLError();
			glDrawArrays(GL_TRIANGLES, 0, range - offset); CheckGLError();
			offset = range;
			m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, offset);
			//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.view_uniform, view);
			//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.proj_uniform, proj);
			//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.sprite_texture_uniform, 0);
		}
	}
	if (m_renderable_data.staging_buffer.size() > 0) {
		range = m_renderable_data.staging_buffer.size();
		glActiveTexture(GL_TEXTURE0); CheckGLError();
		glBindTexture(GL_TEXTURE_2D, m_renderable_data.staging_buffer[m_renderable_data.staging_buffer.size() - 1].texture_id); CheckGLError();
		glDrawArrays(GL_TRIANGLES, 0, range - offset); CheckGLError();
		m_renderable_draw_params.shader->setValue(m_renderable_draw_params.offset_uniform, 0);
		//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.view_uniform, view);
		//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.proj_uniform, proj);
		//m_renderable_draw_params.shader->setValue(m_renderable_draw_params.sprite_texture_uniform, 0);
	}*/

	m_filter_draw_params.shader->bind();
	m_renderable_draw_params.shader->setValue(m_filter_draw_params.render_texture_uniform, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); CheckGLError();
	glClear(GL_COLOR_BUFFER_BIT); CheckGLError();
	glActiveTexture(GL_TEXTURE0); CheckGLError();
	glUniform1i(0, 0); CheckGLError();
	glBindTexture(GL_TEXTURE_2D, m_framebuffer.texture); CheckGLError();
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1); CheckGLError();

}

void Renderer::shutdown() {

	glBindVertexArray(0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	u32 handles[2] = { m_renderable_data.shader_handle, m_vertex_dummies.vbo };
	glDeleteBuffers(2, handles);
	glDeleteFramebuffers(1, &m_framebuffer.handle);
	glDeleteVertexArrays(1, &m_vertex_dummies.vao);
	glDeleteTextures(1, &m_framebuffer.texture);

}