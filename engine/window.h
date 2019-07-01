#pragma once

#include "common.h"
#include "frame.h"
#include "config.h"

#include "glad/glad.h"
#include "SDL2/SDL.h"

class Window {
public:
	void startup(FrameContext& frame_context);
	void shutdown();
	void recordInput();
	b32 quitEvent();
	void show();
	void hide();
	void swap();
private:
	FrameContext* m_frame_context = nullptr;
	SDL_Window* m_handle = nullptr;
	SDL_GLContext m_context;
	b32 m_quit_event = false;
	b32 m_fullscreen = false;
	u8 m_cur_input_state[SDL_NUM_SCANCODES];
	u8 m_prev_input_state[SDL_NUM_SCANCODES];
};