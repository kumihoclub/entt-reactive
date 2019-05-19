
#include "timing.h"

#include <vector>

namespace {

	std::vector<Timing::Timer*> timers;
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
	// For uniformities sakes
}

void Timing::Manager::update(f64 new_delta_time) {
	delta_time = new_delta_time;
	for (auto& timer : timers) {
		if (timer->running()) {
			timer->m_incremented_time -= delta_time;
		}
	}

}

void Timing::Manager::shutdown() {
	// for uniformities sakes
}

f64 Timing::deltaTime() {
	return delta_time;
}