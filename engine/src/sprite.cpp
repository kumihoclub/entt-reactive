
#include "sprite.h"

#include "glm/gtc/matrix_transform.hpp"

namespace {

	struct SpriteDestructionPending {};

	entt::registry* stored_reg = nullptr;
	b32 is_setup = false;

	struct {
		entt::observer sprite_create;
		entt::observer sprite_change;
		entt::observer transform_change;
	} observers;

	std::vector<u32> pending_sprite_destructions;

}

/*void Sprite::Management::setupCommandHandling(entt::registry& reg) {

	auto sprite_create = entt::collector.group<Sprite::Root>().where<Transform>();
	auto sprite_change = entt::collector.replace<Sprite::Root>().where<Transform>();
	auto transform_change = entt::collector.replace<Transform>().where<Sprite::Root>();

	observers.sprite_create.connect(reg, sprite_create);
	observers.sprite_change.connect(reg, sprite_change);
	observers.transform_change.connect(reg, transform_change);
	
	pending_sprite_destructions.reserve(4000);

	reg.on_destroy<Sprite::Root>().connect < +[](entt::registry& reg, entt::entity entity) -> void {
		// If the entity doesnt have a Transform then a sprite renderable was never created, so dont queue a destruction command
		if (reg.has<Transform>(entity)) {
			pending_sprite_destructions.push_back(entity);
		}
	} > ();


	stored_reg = &reg;
	is_setup = true;

}

void Sprite::Management::recordCommands(Frame& frame) {
	if (is_setup) {

		observers.sprite_create.each([&](const auto entity) {
			Transform transform = stored_reg->get<Transform>(entity);
			glm::mat4 render_transform = glm::mat4(1.0f);
			render_transform = glm::translate(render_transform, glm::vec3(transform.pos, 0.0f));
			render_transform = glm::rotate(render_transform, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
			render_transform = glm::scale(render_transform, glm::vec3(transform.scale, 0.0f));
			frame.createSprite(entity, transform.layer, render_transform, stored_reg->get<Sprite::Root>(entity));
		});

		observers.sprite_change.each([&](const auto entity) {
			Sprite::Root material = stored_reg->get<Sprite::Root>(entity);
			frame.updateSpriteMaterial(entity, material);
		});

		observers.transform_change.each([&](const auto entity) {
			Transform transform = stored_reg->get<Transform>(entity);
			glm::mat4 render_transform = glm::mat4(1.0f);
			render_transform = glm::translate(render_transform, glm::vec3(transform.pos, 0.0f));
			render_transform = glm::rotate(render_transform, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
			render_transform = glm::scale(render_transform, glm::vec3(transform.scale, 0.0f));
			frame.updateSpriteTransform(entity, render_transform);
		});

		/*for (const auto entity : observers.sprite_create) {
			Transform transform = stored_reg->get<Transform>(entity);
			glm::mat4 render_transform = glm::mat4(1.0f);
			render_transform = glm::translate(render_transform, glm::vec3(transform.pos, 0.0f));
			render_transform = glm::rotate(render_transform, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
			render_transform = glm::scale(render_transform, glm::vec3(transform.scale, 0.0f));
			frame.createSprite(entity, transform.layer, render_transform, stored_reg->get<Sprite::Root>(entity));
		}

		for (const auto entity : observers.sprite_change) {
			Sprite::Root material = stored_reg->get<Sprite::Root>(entity);
			frame.updateSpriteMaterial(entity, material);
		}

		for (const auto entity : observers.transform_change) {
			Transform transform = stored_reg->get<Transform>(entity);
			glm::mat4 render_transform = glm::mat4(1.0f);
			render_transform = glm::translate(render_transform, glm::vec3(transform.pos, 0.0f));
			render_transform = glm::rotate(render_transform, transform.rot, glm::vec3(0.0f, 0.0f, 1.0f));
			render_transform = glm::scale(render_transform, glm::vec3(transform.scale, 0.0f));
			frame.updateSpriteTransform(entity, render_transform);
		}

		for (const auto entity : pending_sprite_destructions) {
			frame.destroySprite(entity);
		}
		pending_sprite_destructions.resize(0);

	}
}*/
