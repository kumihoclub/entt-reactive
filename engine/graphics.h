#pragma once

#include "common.h"
#include "frame.h"
#include "shader.h"
#include "sprite_renderable.h"
#include "sprite_cache.h"
#include "sprite_renderer.h"

#include "glad/glad.h"

#include <optional>

class Graphics {
public:
	void startup(FrameContext& frame_context);
	void shutdown();
	void draw();
	b32 running();
private:

	SpriteCache m_sprite_cache;
	SpriteRenderer m_sprite_renderer;

	struct {
		u32 handle;
		u32 texture;
	} m_framebuffer;

	FrameContext* m_frame_context = nullptr;

};