
#include "glad/glad.h"
#include "glad/src/glad.c"

#include "window.h"

#include <iostream>


void Window::startup(FrameContext& frame_context) {

	memset(m_cur_input_state, 0, SDL_NUM_SCANCODES);
	memset(m_prev_input_state, 0, SDL_NUM_SCANCODES);

	m_frame_context = &frame_context;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

	const Config& config = Config::get();
	m_handle = SDL_CreateWindow(config.title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		config.window_size.x, config.window_size.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);

	m_context = SDL_GL_CreateContext(m_handle);

	SDL_GL_MakeCurrent(m_handle, m_context);

	if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, config.window_size.x, config.window_size.y);

}

void Window::shutdown() {

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_handle);

}

void Window::recordInput() {

	//m_frame_context->frontFrame().input_buffer.resize(0);
	int num_keys;
	SDL_GetKeyboardState(&num_keys);
	memcpy(m_prev_input_state, m_cur_input_state, num_keys);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN) {
			if (e.type == SDL_KEYUP && e.key.keysym.scancode == SDL_SCANCODE_F10) {
				const Config& config = Config::get();
				if (m_fullscreen == true) {
					SDL_ShowCursor(SDL_ENABLE);
					glViewport(0, 0, config.window_size.x, config.window_size.y);
					SDL_SetWindowFullscreen(m_handle, 0);
				}
				else {
					SDL_ShowCursor(SDL_DISABLE);
					SDL_DisplayMode current;
					SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(m_handle), &current);
					glViewport(0, 0, current.w, current.h);
					SDL_SetWindowFullscreen(m_handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				m_fullscreen = !m_fullscreen;
			}
			//m_frame_context->frontFrame().input_buffer.push_back(e);
		}
		else if (e.type == SDL_QUIT) {
			m_quit_event = true;
		}
	}
	const u8* keystate = SDL_GetKeyboardState(&num_keys);
	memcpy(m_cur_input_state, keystate, num_keys);
	m_frame_context->frontFrame().input.set(m_cur_input_state, m_prev_input_state, num_keys);
}

b32 Window::quitEvent() {
	return m_quit_event;
}


void Window::show() {
	SDL_ShowWindow(m_handle);
}

void Window::hide() {
	SDL_HideWindow(m_handle);
}

void Window::swap() {
	SDL_GL_SwapWindow(m_handle);
}