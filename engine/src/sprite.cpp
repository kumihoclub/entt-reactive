
#include "sprite.h"

Sprite::Sprite() {}

void Sprite::pushFrame(Frame frame) {
	if ((m_frame_count + 1) < 16) {
		m_frames[m_frame_count + 1] = frame;
		m_frame_count++;
	}
	if (m_frame_count == 1) {
		m_cur_frame = 0;
	}
}

void Sprite::setFrame(u32 index) {
	if (index < 16 && (m_cur_frame + 1) <= m_frame_count) {
		m_cur_frame = index;
	}
}

std::optional<Sprite::Frame> Sprite::getFrame() {
	if (m_frame_count > 0) {
		return m_frames[m_cur_frame];
	}
	return std::nullopt;
}

u32 Sprite::frameCount() {
	return m_frame_count;
}