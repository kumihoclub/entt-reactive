#pragma once

#include "common.h"
#include "frame.h"
#include "shader.h"
#include "renderable.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <thread>
#include <atomic>
#include <condition_variable>

// No managers here, just the renderer, just the devil himself

class Renderer {
public:
	void init();
	b32 is_drawing();
	void wait();
	void draw(Frame& frame);
	void shutdown();
private:

	u32 m_vao = 0;
	GLFWwindow* m_context = nullptr;
	std::thread m_thread;
	std::atomic_bool m_is_drawing = false;
	std::atomic_bool m_ready_to_draw = false;
	std::atomic_bool m_running = false;
	std::atomic_bool m_context_taken = false;
	std::mutex m_condition_lock;
	std::condition_variable m_condition_var;

	Frame* m_frame = nullptr;

	struct {
		std::vector<Renderable> staging_buffer;
		u32 shader_handle;
		u32 shader_buffer_capacity = 0;
		handle<Shader> shader;
		i32 offset_uniform = 0;
		i32 view_uniform = 0;
		i32 proj_uniform = 0;
		i32 sprite_texture_uniform = 0;
	} m_renderable;

	struct {
		handle<Shader> shader;
		i32 render_texture_uniform = 0;
	} m_filter;

	struct {
		u32 handle;
		u32 texture;
	} m_framebuffer;

};