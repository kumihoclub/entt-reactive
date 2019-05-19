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

struct DrawCommand {
	Transform transform;
	Sprite sprite;
};

class Frame {
public:
	void createBackgroundDrawCommand(Transform& transform, Sprite& sprite);
	void createWorldDrawCommand(Transform& transform, Sprite& sprite);
	void createGuiDrawCommand(Transform& transform, Sprite& sprite);
	void consumeBackgroundDrawCommands(std::vector<Renderable>& staging_buffer);
	void consumeWorldDrawCommands(std::vector<Renderable>& staging_buffer);
	void consumeGuiDrawCommands(std::vector<Renderable>& staging_buffer);
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
	void consumeCommands(std::vector<Renderable>& staging_buffer, std::vector<DrawCommand>& command_buffer);
	std::vector<DrawCommand> m_background_commands;
	std::vector<DrawCommand> m_world_commands;
	std::vector<DrawCommand> m_gui_commands;
};