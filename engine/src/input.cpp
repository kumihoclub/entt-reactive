
#include "input.h"

InputState::InputState() {}

void InputState::set(const u8* const cur, const u8* const prev, u32 num_keys) {
	memcpy(m_cur_state, cur, num_keys);
	memcpy(m_prev_state, prev, num_keys);
}

b32 InputState::pressed(u8 scancode) {
	return (m_cur_state[scancode] && !m_prev_state[scancode]);
}

b32 InputState::held(u8 scancode) {
	return (m_cur_state[scancode] && m_prev_state[scancode]);
}

b32 InputState::released(u8 scancode) {
	return (!m_cur_state[scancode] && m_prev_state[scancode]);
}