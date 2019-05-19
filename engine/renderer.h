#pragma once

#include "common.h"
#include "frame.h"
#include "shader.h"
#include "renderable.h"

#include "glad/glad.h"

// No managers here, just the renderer, just the devil himself

class Renderer {
public:
	void init();
	void draw(Frame& frame);
	void shutdown();
private:

	struct {
		u32 vbo = 0;
		u32 vao = 0;
	} m_vertex_dummies;

	struct {
		std::vector<Renderable> staging_buffer;
		u32 shader_handle;
		u32 shader_buffer_capacity = 0;
	} m_renderable_data;

	struct {
		handle<Shader> shader;
		i32 offset_uniform = 0;
		i32 view_uniform = 0;
		i32 proj_uniform = 0;
		i32 sprite_texture_uniform = 0;
	} m_renderable_draw_params;

	struct {
		handle<Shader> shader;
		i32 render_texture_uniform = 0;
	} m_filter_draw_params;

	struct {
		u32 handle;
		u32 texture;
	} m_framebuffer;
};