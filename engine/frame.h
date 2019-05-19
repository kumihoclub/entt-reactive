#pragma once

#include "transform.h"
#include "sprite.h"
#include "camera.h"
#include "shader.h"

#include <vector>
#include "renderable.h"
#include "transform.h"
#include "sprite.h"

#include <optional>
#include <atomic>
#include <mutex>

struct DrawCommand {
	Transform transform;
	Sprite sprite;
};

class Frame {
public:
	Frame();
	void createDrawCommand(Transform& transform, Sprite& sprite);
	void consumeDrawCommands(std::vector<Renderable>& staging_buffer);
	struct {
		glm::vec4 environment = { 1.0f, 1.0f, 1.0f, 1.0f };
		f32 glitch_strength = 0.0f;
	} background_params;
	struct {
		glm::vec4 environment = { 1.0f, 1.0f, 1.0f, 1.0f };
		f32 glitch_strength = 0.0f;
	} world_filter_params;
	struct {
		glm::vec4 environment = { 1.0f, 1.0f, 1.0f, 1.0f };
		f32 glitch_strength = 0.0f;
	} gui_filter_params;
private:
	void consumeCommands(std::vector<Renderable>& staging_buffer);
	std::vector<DrawCommand> m_commands;
	std::mutex m_comsume_lock;
};