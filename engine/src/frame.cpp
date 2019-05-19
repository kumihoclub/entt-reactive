
#include "frame.h"

#include "glm/gtc/matrix_transform.hpp"

void Frame::createBackgroundDrawCommand(Transform& transform, Sprite& sprite) {
	m_background_commands.push_back({ transform, sprite });
}

void Frame::createWorldDrawCommand(Transform& transform, Sprite& sprite) {
	m_world_commands.push_back({ transform, sprite });
}

void Frame::createGuiDrawCommand(Transform& transform, Sprite& sprite) {
	m_gui_commands.push_back({ transform, sprite });
}

void Frame::consumeBackgroundDrawCommands(std::vector<Renderable>& staging_buffer) {
	consumeCommands(staging_buffer, m_background_commands);
}

void Frame::consumeWorldDrawCommands(std::vector<Renderable>& staging_buffer) {
	consumeCommands(staging_buffer, m_world_commands);
}

void Frame::consumeGuiDrawCommands(std::vector<Renderable>& staging_buffer) {
	consumeCommands(staging_buffer, m_gui_commands);
}

void Frame::consumeCommands(std::vector<Renderable>& staging_buffer, std::vector<DrawCommand>& command_buffer) {

	// Sort commands
	std::sort(command_buffer.begin(), command_buffer.end(), [](const DrawCommand & a, const DrawCommand & b) -> bool
		{
			if (a.transform.layer < b.transform.layer)
				return true;
			else if (b.transform.layer < a.transform.layer)
				return false;
			else if (a.sprite.texture.id < b.sprite.texture.id)
				return true;
			else if (b.sprite.texture.id < a.sprite.texture.id)
				return false;
		});

	staging_buffer.resize(command_buffer.size());

	for (int i = 0; i < command_buffer.size(); ++i) {
		DrawCommand& command = command_buffer[i];
		Renderable& renderable = staging_buffer[i];

		renderable.model = glm::mat4(1.0f);
		glm::vec3 test = glm::vec3(command.transform.pos * UNIT_PIXELS, 0.0f);
		renderable.model = glm::translate(renderable.model, glm::vec3(command.transform.pos * UNIT_PIXELS, 0.0f));
		renderable.model = glm::rotate(renderable.model, command.transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
		renderable.model = glm::scale(renderable.model, glm::vec3((f32)(command.transform.scale.x * command.sprite.size.x), (f32)(command.transform.scale.y * command.sprite.size.y), 0.0f));

		/*
		vec2 uvs[6] = {
			// first triangle
			vec2(1.0f, 1.0f),  // top right
			vec2(1.0f, 0.0f),  // bottom right
			vec2(0.0f, 1.0f),  // top left 
			// second triangle
			vec2(1.0f, 0.0f),  // bottom right
			vec2(0.0f, 0.0f),  // bottom left
			vec2(0.0f, 1.0f)   // top left
		}
		*/

		f32 u = (f32)(command.sprite.uv.x / (f32)command.sprite.texture.size.x);
		f32 v = (f32)(command.sprite.uv.y / (f32)command.sprite.texture.size.x);
		f32 w = ((f32)command.sprite.size.x / (f32)command.sprite.texture.size.x);
		f32 h = ((f32)command.sprite.size.y / (f32)command.sprite.texture.size.y);

		renderable.uv[0] = { u + w, 1.0f - (v) };
		renderable.uv[1] = { u + w, 1.0f - (v + h) };
		renderable.uv[2] = { u , 1.0f - (v) };
		renderable.uv[3] = { u + w, 1.0f - (v + h) };
		renderable.uv[4] = { u , 1.0f - (v + h) };
		renderable.uv[5] = { u , 1.0f - (v) };

		/*renderable.uv[0] = { u + w, 1.0f - (v + h) };
		renderable.uv[1] = { u + w, 1.0f - (v) };
		renderable.uv[2] = { u, 1.0f - (v + h) };

		renderable.uv[3] = { u + w, 1.0f - (v) };
		renderable.uv[4] = { u , 1.0f - (v) };
		renderable.uv[5] = { u , 1.0f - (v + h) };*/

		renderable.color = command.sprite.color;
		renderable.glitch = command.sprite.glitch;
		renderable.texture_id = command.sprite.texture.id;
	}

	command_buffer.resize(0);
}