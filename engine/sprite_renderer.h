#pragma once

#include "common.h"
#include "config.h"
#include "frame.h"
#include "shader.h"
#include "sprite_cache.h"
#include "sprite_renderable.h"

#include <optional>
#include <vector>
#include <tuple>

class SpriteRenderer {
public:
	void startup();
	void shutdown();
	void draw(Frame& frame, SpriteCache& cache);
private:
	std::optional<u32> m_vao;
	struct {
		std::optional<u32> view_proj_uni;
		std::optional<u32> transform;
		std::optional<u32> material;
		std::optional<u32> draw;
	} m_buffers;
	Shader m_shader;
	std::vector<u32> m_renderable_stack;
	std::vector<u32> m_draw_buffer;
	std::vector<std::tuple<u32, u32, u32>> m_active_renderables;
};

/*namespace Sprite {

	class Renderer {
	public:
		void startup();
		void shutdown();
		void draw(Frame& frame);
	private:
		struct {
			std::optional<u32> view_proj_uni;
			std::optional<u32> transform;
			std::optional<u32> material;
			std::optional<u32> draw;
		} m_buffers;
		Shader m_shader;
		std::vector<u32> m_renderable_stack;
		std::vector<u32> m_draw_buffer;
		std::vector<std::tuple<u32, u32, u32>> m_active_renderables;
	};

}*/