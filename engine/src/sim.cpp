
#include "sim.h"

#include "Box2D/Box2D.h"

void Sim::startup(FrameContext& frame) {

	m_frame_context = &frame;

	m_physics_world = new b2World(b2Vec2(0.0f, 0.0f));
	entt::registry& reg = Registry::get();
	entt::prototype player_prototype{ reg };
	Player::formatPrototype(player_prototype);
	m_player = player_prototype.create();
	Velocity vel;
	vel.linear = { 0.025f, 0.0f };
	reg.assign<Velocity>(m_player, vel);
	//Transform transform;
	//transform.pos = { 1.0f, 0.0 };
	//reg.replace<Transform>(m_player, transform);

	m_async.sim_running = true;

	m_sim_thread = std::thread([&]() {
		f64 timestep = (1.0 / 60.0);
		timestamp cur_time = std::chrono::high_resolution_clock::now();;
		while (m_async.sim_running) {
			std::unique_lock<std::mutex> l(m_async.frame_active_lock);
			m_async.frame_active_cond.wait(l, [&]() {
				if (!m_async.sim_running.load()) { return true; }
				return (bool)m_async.frame_active_val.load();
			});
			if (!m_async.sim_running.load()) {
				return 0;
			}
			timestamp new_time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<f64, std::milli> duration = (new_time - cur_time);
			f64 frame_time = duration.count();
			cur_time = new_time;
			while (frame_time > 0.0) {
				f64 delta_time = std::min(frame_time, timestep);
				m_frame_context->backFrame().delta_time = delta_time;

				// TODO: Actual step func
				m_physics.step(reg, delta_time);

				frame_time -= delta_time;
			}
			m_frame_context->backFrame().recordCommands();
			m_async.frame_active_val = false;
			m_async.frame_active_cond.notify_all();
		}
	});

}

void Sim::shutdown() {

	m_async.sim_running = false;
	m_async.frame_active_cond.notify_all();
	m_sim_thread.join();
	delete m_physics_world;

}

void Sim::step() {
	std::unique_lock<std::mutex> l(m_async.frame_active_lock);
	m_async.frame_active_val = true;
	l.unlock();
	m_async.frame_active_cond.notify_all();
}

void Sim::wait() {
	std::unique_lock<std::mutex> l(m_async.frame_active_lock);
	m_async.frame_active_cond.wait(l, [this]() {
		return (m_async.frame_active_val.load() == false); 
	});
}