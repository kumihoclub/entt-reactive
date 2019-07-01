
#include "registry.h"

entt::registry& Registry::get() {
	static entt::registry registry;
	return registry;
}