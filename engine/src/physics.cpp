
#include "physics.h"

#include <iostream>

/*void PhysicsEngine::startup() {

}

void PhysicsEngine::shutdown() {

}*/

void PhysicsEngine::step(entt::registry& registry, f64 delta) {

	// Transform cannot be a ref and has to be replaced for Observers to be updated
	registry.view<Transform, Velocity>().each([&](auto entity, auto transform, auto& velocity) {
		transform.pos.x += (velocity.linear.x * delta);
		transform.pos.y += (velocity.linear.y * delta);
		registry.replace<Transform>(entity, transform);
	});

}

const std::vector<Collision>& PhysicsEngine::getCollisionBuffer() {
	return m_collision_buffer;
}