#pragma once

#include "common.h"

#include "SDL2/SDL.h"

class InputState {
public:
	InputState();
	void set(const u8* const cur, const u8* const prev, u32 num_keys);
	b32 pressed(u8 scancode);
	b32 held(u8 scancode);
	b32 released(u8 scancode);
private:
	u8 m_prev_state[SDL_NUM_SCANCODES];
	u8 m_cur_state[SDL_NUM_SCANCODES];
};
