#pragma once

#include "common.h"
#include "transform.h"

#include "cute/cute_c2.h"
#include "entt/entt.hpp"

#include <vector>

enum class ColliderShape {
	BOX = C2_AABB,
	CIRCLE = C2_CIRCLE,
	CAPSULE = C2_CAPSULE
};

enum class ColliderType {
	STATIC,
	DYNAMIC
};

struct Collider {
	ColliderShape shape = ColliderShape::BOX;
	ColliderType type = ColliderType::STATIC;
	union {
		c2AABB box;
		c2Circle circle;
		c2Ray ray;
		c2Capsule capsule;
	};
};

struct Collision {
	entt::entity entitys[2];
};

class PhysicsEngine {
public:
	//void startup();
	//void shutdown();
	void step(entt::registry& registry, f64 delta);
	const std::vector<Collision>& getCollisionBuffer();
private:
	std::vector<Collision> m_collision_buffer;
};