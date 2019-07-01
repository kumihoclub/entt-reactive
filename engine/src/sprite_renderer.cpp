
#include "sprite_renderer.h"

#include "glad/glad.h"
#include "gl_err.h"

void SpriteRenderer::startup() {

	u32 vao;
	glGenVertexArrays(1, &vao); CheckGLError();
	m_vao = vao;

	// Generate and populate buffers

	u32 buffers[4];
	glCreateBuffers(4, buffers); CheckGLError();
	m_buffers.view_proj_uni = buffers[0];
	m_buffers.transform = buffers[1];
	m_buffers.material = buffers[2];
	m_buffers.draw = buffers[3];

	std::vector<glm::mat4> transform_default;
	std::vector<SpriteRenderable> renderable_default;
	transform_default.resize(4000);
	renderable_default.resize(4000);
	for (auto& transform : transform_default) {
		transform = glm::mat4(1.0);
	}

	glm::mat4 view_proh_default[2] = { glm::mat4(1.0), glm::mat4(1.0) };
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffers.view_proj_uni.value()); CheckGLError();
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, view_proh_default, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers.transform.value()); CheckGLError();
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * transform_default.size(), transform_default.data(), GL_DYNAMIC_DRAW); CheckGLError();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers.material.value()); CheckGLError();
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(SpriteRenderable) * renderable_default.size(), renderable_default.data(), GL_DYNAMIC_DRAW); CheckGLError();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers.draw.value()); CheckGLError();
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(u32) * transform_default.size(), nullptr, GL_DYNAMIC_DRAW); CheckGLError();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); CheckGLError();

	std::string vert =
#include "shader/draw.vert"
	;
	std::string frag =
#include "shader/draw.frag" 
	;

	const Config& config = Config::get();

	std::string sprite_capacity_search = "%SPRITE_RENDERABLE_CAPACITY%";
	std::string replace_string = std::to_string(config.sprite_renderable_capacity);

	size_t pos = vert.find(sprite_capacity_search);
	while (pos != std::string::npos) {
		vert.replace(pos, sprite_capacity_search.size(), replace_string);
		pos = vert.find(sprite_capacity_search, pos + replace_string.size());
	}

	pos = frag.find(sprite_capacity_search);
	while (pos != std::string::npos) {
		frag.replace(pos, sprite_capacity_search.size(), replace_string);
		pos = frag.find(sprite_capacity_search, pos + replace_string.size());
	}

	m_shader = Shader(vert, frag);

	m_active_renderables.reserve(4000);
	m_draw_buffer.reserve(4000);
	m_renderable_stack.resize(4000);
	for (int i = 0; i < m_renderable_stack.size(); ++i) {
		m_renderable_stack[i] = m_renderable_stack.size() - 1 - i;
	}

}

void SpriteRenderer::shutdown() {

	m_shader.destroy();
	u32 buffers[4];
	buffers[0] = m_buffers.view_proj_uni.value();
	buffers[1] = m_buffers.transform.value();
	buffers[2] = m_buffers.material.value();
	buffers[3] = m_buffers.draw.value();
	glDeleteBuffers(4, buffers); CheckGLError();
	glDeleteVertexArrays(1, &m_vao.value());

}

void SpriteRenderer::draw(Frame& frame, SpriteCache& cache) {

	glBindVertexArray(m_vao.value());

	// If there are any creation/destruction events then the draw buffer needs to be updated potentially with sorting

	b32 needs_draw_regen = false;
	b32 needs_draw_sorting = false;

	if (frame.sprite.create_commands.size() > 0 || frame.sprite.destroy_commands.size() > 0) {
		needs_draw_regen = true;
	}
	if (frame.sprite.create_commands.size() > 0) {
		needs_draw_sorting = true;
	}

	// Process create/destroy commands

	for (auto& command : frame.sprite.destroy_commands) {
		for (int i = 0; i < m_active_renderables.size(); ++i) {
			u32 id, layer, spr;
			std::tie(id, layer, spr) = m_active_renderables[i];
			if (command == id) {
				m_active_renderables.erase(m_active_renderables.begin() + i);
				m_renderable_stack.push_back(spr);
				for (int x = 0; x < m_draw_buffer.size(); ++x) {
					if (m_draw_buffer[x] == spr) {
						m_draw_buffer.erase(m_draw_buffer.begin() + x);
						break;
					}
				}
				break;
			}
		}
	}
	frame.sprite.destroy_commands.resize(0);

	for (auto& command : frame.sprite.create_commands) {
		u32 id;
		u32 layer;
		glm::mat4 transform;
		SpriteRenderable renderable;
		std::tie(id, layer, transform, renderable) = command;
		u32 spr = m_renderable_stack.back();
		m_renderable_stack.pop_back();
		m_active_renderables.push_back({ id, layer, spr });
		frame.sprite.transform_updates.push_back({ id, transform });
		frame.sprite.renderable_updates.push_back({ id, renderable });
	}
	frame.sprite.create_commands.resize(0);

	// Bind the cache

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cache.getShaderSpriteBuffer().value());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cache.getShaderSpriteBuffer().value()); CheckGLError();

	// Process updates while also binding the bases 

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers.transform.value()); CheckGLError();
	for (auto& update : frame.sprite.transform_updates) {
		u32 update_id;
		glm::mat4 transform;
		std::tie(update_id, transform) = update;
		for (auto& renderable : m_active_renderables) {
			u32 id;
			u32 index;
			u32 layer;
			std::tie(id, layer, index) = renderable;
			if (id == update_id) {
				glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * index, sizeof(glm::mat4), &transform); CheckGLError();
				break;
			}
		}
	}
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_buffers.transform.value()); CheckGLError();
	frame.sprite.transform_updates.resize(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers.material.value()); CheckGLError();
	for (auto& update : frame.sprite.renderable_updates) {
		u32 update_id;
		SpriteRenderable renderable;
		std::tie(update_id, renderable) = update;
		for (auto& active_renderable : m_active_renderables) {
			u32 id;
			u32 index;
			u32 layer;
			std::tie(id, layer, index) = active_renderable;
			if (id == update_id) {
				glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(SpriteRenderable) * index, sizeof(SpriteRenderable), &renderable);
				break;
			}
		}
	}
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_buffers.material.value()); CheckGLError();
	frame.sprite.renderable_updates.resize(0);

	// Sort and upload if needed

	if (needs_draw_sorting) {
		std::sort(m_active_renderables.begin(), m_active_renderables.end(), [](const std::tuple<u32, u32, u32> & a, const std::tuple<u32, u32, u32> & b) -> bool {
			u32 id_a, layer_a, spr_a, id_b, layer_b, spr_b;
			std::tie(id_a, layer_a, spr_a) = a;
			std::tie(id_b, layer_b, spr_b) = b;
			if (layer_a < layer_b)
				return true;
			return false;
		});
	}

	if (needs_draw_regen) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers.draw.value()); CheckGLError();
		m_draw_buffer.resize(0);
		for (auto& renderable : m_active_renderables) {
			u32 id;
			u32 index;
			u32 layer;
			std::tie(id, layer, index) = renderable;
			m_draw_buffer.push_back(index);
		}
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(u32)* m_draw_buffer.size(), m_draw_buffer.data()); CheckGLError();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_buffers.draw.value()); CheckGLError();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); CheckGLError();
	}
	
	glBindBuffer(GL_UNIFORM_BUFFER, m_buffers.view_proj_uni.value()); CheckGLError();
	glm::mat4 new_view_proj[2] = { frame.view_proj.view, frame.view_proj.proj }; CheckGLError();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4) * 2, new_view_proj); CheckGLError();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_buffers.view_proj_uni.value()); CheckGLError();

	// Draw frame

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, cache.getCacheTexture().value());
	glUseProgram(m_shader.handle().value());
	glDrawArrays(GL_TRIANGLES, 0, m_draw_buffer.size() * 6);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	glBindVertexArray(0);

}