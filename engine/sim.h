#pragma once

#include "common.h"
#include "frame.h"
#include "transform.h"
#include "sprite_renderable.h"
#include "sprite_cache.h"
#include "player.h"
#include "registry.h"
#include "physics.h"

#include "Box2D/Box2D.h"
#include "entt/entt.hpp"
#include "entt/entity/observer.hpp"

#include <chrono>
#include <ratio>
#include <thread>
#include <atomic>

using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Sim {
public:
	void startup(FrameContext& frame);
	void shutdown();
	void step();
	void wait();
	b32 running();
private:
	PhysicsEngine m_physics;
	std::thread m_sim_thread;
	entt::entity m_player = entt::null;
	b2World* m_physics_world = nullptr;
	FrameContext* m_frame_context = nullptr;
	struct {
		std::atomic<b32> sim_running = false;
		std::atomic<b32> frame_active_val = false;
		std::condition_variable frame_active_cond;
		std::mutex frame_active_lock;
	} m_async;
};