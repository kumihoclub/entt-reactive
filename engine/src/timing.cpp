
#include "timing.h"

#include <vector>

namespace {

	std::vector<Timing::Timer*> timers;
	f64 prev_time = 0.0;
	f64 cur_time = 0.0;
	f64 delta_time = 0.0;

}

Timing::Timer::Timer() {
	timers.push_back(this);
}

Timing::Timer::~Timer() {
	for (int i = 0; i < timers.size(); ++i) {
		if (timers[i] == this) {
			timers.erase(timers.begin() + i);
			return;
		}
	}
}

void Timing::Timer::setTime(f64 target_time) {
	m_target_time = target_time;
	reset();
}

void Timing::Timer::reset() {
	m_incremented_time = m_target_time;
}

b32 Timing::Timer::triggered() {
	return (m_incremented_time <= 0.0);
}

b32 Timing::Timer::running() {
	return m_running;
}

void Timing::Timer::run() {
	m_running = true;
}

void Timing::Timer::pause() {
	m_running = false;
}

void Timing::Manager::init() {
	// for uniformities sakes
}

void Timing::Manager::update() {
	cur_time = glfwGetTime();
	delta_time = cur_time - prev_time;
	prev_time = cur_time;
	for (auto& timer : timers) {
		if (timer->running()) {
			timer->m_incremented_time -= delta_time;
		}
	}
}

void Timing::Manager::shutdown() {
	// for uniformities sakes
}

f64 Timing::getDeltaTime() {
	return delta_time;
}