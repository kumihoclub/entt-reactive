#pragma once

#include "common.h"
#include "frame.h"
#include "animation.h"
#include "sprite_renderable.h"
#include "sprite_cache.h"
#include "registry.h"

#include "entt/entt.hpp"

namespace Player {

	struct Root {
		enum Direction {
			UP, DOWN, LEFT, RIGHT
		};
		Direction direction = DOWN;
		u32 walk_anim_frame_index = 0;
		f64 walk_anim_time = 0.1 / 1000;
		f64 walk_anim_timestamp = 0.0;
		b32 walking = false;
	};

	struct ControlsDisabled {};

	inline void formatPrototype(entt::prototype& prototype) {
		Animation::WalkFrames walk_frames;
		walk_frames.up[0] = SpriteCache::fetchSprite("mako_movement_9.png").value();
		walk_frames.up[1] = SpriteCache::fetchSprite("mako_movement_10.png").value();
		walk_frames.up[2] = SpriteCache::fetchSprite("mako_movement_11.png").value();
		walk_frames.left[0] = SpriteCache::fetchSprite("mako_movement_6.png").value();
		walk_frames.left[1] = SpriteCache::fetchSprite("mako_movement_7.png").value();
		walk_frames.left[2] = SpriteCache::fetchSprite("mako_movement_8.png").value();
		walk_frames.down[0] = SpriteCache::fetchSprite("mako_movement_0.png").value();
		walk_frames.down[1] = SpriteCache::fetchSprite("mako_movement_1.png").value();
		walk_frames.down[2] = SpriteCache::fetchSprite("mako_movement_2.png").value();
		walk_frames.right[0] = SpriteCache::fetchSprite("mako_movement_3.png").value();
		walk_frames.right[1] = SpriteCache::fetchSprite("mako_movement_4.png").value();
		walk_frames.right[2] = SpriteCache::fetchSprite("mako_movement_5.png").value();
		prototype.set<Animation::WalkFrames>(walk_frames);
		SpriteRenderable renderable;
		renderable.sprite_index = walk_frames.down[0];
		prototype.set<SpriteRenderable>(renderable);
		Transform transform;
		transform.scale = { 1.0f, 1.0f };
		prototype.set<Transform>(transform); // ORDERING MATTERS, so set the transform before ANYTHING
		prototype.set<Root>(Root());
	}

}