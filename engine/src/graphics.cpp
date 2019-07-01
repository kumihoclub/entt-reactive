
#include "graphics.h"
#include "window.h"
#include "gl_err.h"

#include "glm/gtc/matrix_transform.hpp"

namespace {

	Graphics* s_graphics_instance;

}

void Graphics::startup(FrameContext& frame_context) {

	m_frame_context = &frame_context;

	s_graphics_instance = this;

	glEnable(GL_BLEND); CheckGLError();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CheckGLError();
	glClearColor(0.0, 0.0, 0.0, 1.0);

	m_sprite_cache.startup();
	m_sprite_renderer.startup();

}

void Graphics::draw() {

	glClear(GL_COLOR_BUFFER_BIT);
	m_sprite_renderer.draw(m_frame_context->frontFrame(), m_sprite_cache);

}

void Graphics::shutdown() {

	m_sprite_renderer.shutdown();
	m_sprite_cache.shutdown();

}