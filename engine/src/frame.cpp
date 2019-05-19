
#include "frame.h"

#include "glm/gtc/matrix_transform.hpp"

Frame::Frame() {
	m_commands.reserve(25000);
}

void Frame::createDrawCommand(Transform& transform, Sprite& sprite) {
	m_comsume_lock.lock();
	m_commands.push_back({ transform, sprite });
	m_comsume_lock.unlock();
}

void Frame::consumeDrawCommands(std::vector<Renderable>& staging_buffer) {
	m_comsume_lock.lock();
	// Sort commands
	std::sort(m_commands.begin(), m_commands.end(), [](const DrawCommand & a, const DrawCommand & b) -> bool
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

	staging_buffer.resize(m_commands.size());

	for (int i = 0; i < m_commands.size(); ++i) {
		DrawCommand& command = m_commands[i];
		Renderable& renderable = staging_buffer[i];

		renderable.model = glm::mat4(1.0f);
		glm::vec3 test = glm::vec3(command.transform.pos * UNIT_PIXELS, 0.0f);
		renderable.model = glm::translate(renderable.model, glm::vec3(command.transform.pos * UNIT_PIXELS, 0.0f));
		renderable.model = glm::rotate(renderable.model, command.transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
		renderable.model = glm::scale(renderable.model, glm::vec3((f32)(command.transform.scale.x * command.sprite.size.x), (f32)(command.transform.scale.y * command.sprite.size.y), 0.0f));

		f32 u = (f32)(command.sprite.uv.x / (f32)command.sprite.texture.size.x);
		f32 v = (f32)(command.sprite.uv.y / (f32)command.sprite.texture.size.x);
		f32 w = ((f32)command.sprite.size.x / (f32)command.sprite.texture.size.x);
		f32 h = ((f32)command.sprite.size.y / (f32)command.sprite.texture.size.y);
		
		renderable.uv = { u, v, w, h };

		renderable.color = command.sprite.color;
		renderable.glitch = command.sprite.glitch;
		renderable.texture_id = command.sprite.texture.id;
	}

	m_commands.resize(0);
	m_comsume_lock.unlock();
}