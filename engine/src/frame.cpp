
#include "frame.h"
#include "sprite_renderable.h"
#include "registry.h"

#include "glm/gtc/matrix_transform.hpp"

#include <mutex>

#include <iostream>

namespace {

	struct IsRendering {};

	struct {
		entt::observer sprite_create;
		entt::observer transform_removed;
		entt::observer sprite_change;
		entt::observer transform_change;
	} observers;

	std::vector<u32> pending_sprite_destructions;

	b32 observers_ready = false;

}

Frame::Frame() {
	sprite.transform_updates.reserve(4000);
	sprite.renderable_updates.reserve(4000);
	sprite.create_commands.reserve(4000);
	sprite.destroy_commands.reserve(4000);
}

void Frame::prepareObservers() {
	if (!observers_ready) {

		entt::registry& reg = Registry::get();

		auto sprite_create = entt::collector.group<SpriteRenderable, Transform>(entt::exclude<IsRendering>);
		auto transform_removed = entt::collector.group<SpriteRenderable>().where<IsRendering>(entt::exclude<Transform>);
		auto sprite_change = entt::collector.replace<SpriteRenderable>().where<Transform>().where<IsRendering>();
		auto transform_change = entt::collector.replace<Transform>().where<SpriteRenderable>().where<IsRendering>();

		observers.sprite_create.connect(reg, sprite_create);
		observers.transform_removed.connect(reg, transform_removed);
		observers.sprite_change.connect(reg, sprite_change);
		observers.transform_change.connect(reg, transform_change);

		pending_sprite_destructions.reserve(4000);

		//reg.on_destroy<IsRendering>().connect<&pushPendingSpriteDestruction>();
		reg.on_destroy<IsRendering>().connect <+[](entt::registry& reg, entt::entity entity) -> void {
			if (reg.has<Transform>(entity)) {
				pending_sprite_destructions.push_back(entity);
			}
		} > ();

		observers_ready = true;
	}
}

void Frame::recordCommands() {

	entt::registry& reg = Registry::get();

	observers.sprite_create.each([&](const auto entity) {
		reg.assign<IsRendering>(entity);
		Transform transform = reg.get<Transform>(entity);
		glm::mat4 render_transform = glm::mat4(1.0f);
		render_transform = glm::translate(render_transform, glm::vec3(transform.pos, 0.0f));
		render_transform = glm::rotate(render_transform, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
		render_transform = glm::scale(render_transform, glm::vec3(transform.scale, 0.0f));
		SpriteRenderable renderable = reg.get<SpriteRenderable>(entity);
		sprite.create_commands.push_back({ entity, transform.layer, render_transform, renderable });
	});

	observers.transform_removed.each([&](const auto entity) {
		reg.remove<IsRendering>(entity);
	});

	observers.sprite_change.each([&](const auto entity) {
		SpriteRenderable renderable = reg.get<SpriteRenderable>(entity);
		sprite.renderable_updates.push_back({ entity, renderable });
	});

	observers.transform_change.each([&](const auto entity) {
		Transform transform = reg.get<Transform>(entity);
		glm::mat4 render_transform = glm::mat4(1.0f);
		render_transform = glm::translate(render_transform, glm::vec3(transform.pos, 0.0f));
		render_transform = glm::rotate(render_transform, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
		render_transform = glm::scale(render_transform, glm::vec3(transform.scale, 0.0f));
		sprite.transform_updates.push_back({ entity, render_transform });
	});

	sprite.destroy_commands.resize(pending_sprite_destructions.size());
	memcpy(sprite.destroy_commands.data(), pending_sprite_destructions.data(), sizeof(u32) * pending_sprite_destructions.size());
	pending_sprite_destructions.resize(0);

}

void FrameContext::startup() {
	Frame::prepareObservers();
	m_front = &m_frames[0];
	m_back = &m_frames[1];
}

void FrameContext::shutdown() {}

Frame& FrameContext::frontFrame() {
	return *m_front;
}

Frame& FrameContext::backFrame() {
	return *m_back;
}

void FrameContext::swapFrames() {
	Frame* swap = m_front;
	m_front = m_back;
	m_back = swap;
}
