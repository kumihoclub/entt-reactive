#pragma once

#include "common.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace Window {

	// Restricted manager class
	class Manager {
	public:
		b32 init(const char* title, u32 w, u32 h);
		void update();
		void shutdown();
		void show();
		void hide();
		void clear();
		void swap();
		b32 quitEvent();
		b32 minimized();
	};

	// Public api

	namespace View {
		u32 getWidth();
		u32 getHeight();
	}

	namespace Control {
		b32 keyPressed(u8 key);
		b32 keyHeld(u8 key);
		b32 keyReleased(u8 key);
	}

}