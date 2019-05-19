#pragma once

// Something something cant name it "time.h" something something meme

#include "common.h"
#include "handle.h"

#include "glfw/glfw3.h"

// In seconds

namespace Timing {

	class Manager;

	class Timer {
	public:
		Timer();
		~Timer();
		void setTime(f64 target_time);
		void reset();
		void run();
		void pause();
		b32 triggered();
		b32 running();
		friend class Timing::Manager;
	private:
		f64 m_incremented_time = 0.0;
		f64 m_target_time = 0.0;
		b32 m_running = false;
	};

	class Manager {
	public:
		void init();
		void update();
		void shutdown();
	};

	f64 getDeltaTime();

}