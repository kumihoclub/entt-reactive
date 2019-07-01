#pragma once

#include "transform.h"

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <optional>
#include <tuple>
#include <atomic>
#include <condition_variable>

#include "common.h"
#include "input.h"

#include "entt/entt.hpp"

// fwddecl
struct SpriteRenderable;

class Frame {
public:
	Frame();
	static void prepareObservers();
	void recordCommands();
	void wait();
	void finish();
	b32 ready();
	f64 delta_time = 0.0;
	InputState input;
	struct {
		std::vector<std::tuple<u32, u32, glm::mat4, SpriteRenderable>> create_commands;
		std::vector<std::tuple<u32, glm::mat4>> transform_updates;
		std::vector<std::tuple<u32, SpriteRenderable>> renderable_updates;
		std::vector<u32> destroy_commands;
	} sprite;
	struct {
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 proj = glm::ortho(-(960.0f / 2), (960.0f / 2), -(540.0f / 2), (540.0f / 2), -64.0f, 64.0f); // TODO: MAKE THIS CONFIGURABLE AND NOT MAGIC!!!!!
	} view_proj;
	friend class FrameContext;
};

class FrameContext {
public:
	void startup();
	void shutdown();
	Frame& frontFrame();
	Frame& backFrame();
	void swapFrames();
private:
	Frame m_frames[2];
	Frame* m_front = nullptr;
	Frame* m_back = nullptr;
};