R"(
#version 430 core

vec2 vertices[6] = {
	// first triangle
	vec2(0.5f,  0.5f),  // top right
	vec2(0.5f, -0.5f),  // bottom right
	vec2(-0.5f,  0.5f),  // top left 
	// second triangle
	vec2(0.5f, -0.5f),  // bottom right
	vec2(-0.5f, -0.5f),  // bottom left
	vec2(-0.5f,  0.5f)   // top left
};

// Flipped on y
vec2 uvs[6] = {
	// first triangle
	vec2(1.0f, 0.0f),  // top right
	vec2(1.0f, 1.0f),  // bottom right
	vec2(0.0f, 0.0f),  // top left 
	// second triangle
	vec2(1.0f, 1.0f),  // bottom right
	vec2(0.0f, 1.0f),  // bottom left
	vec2(0.0f, 0.0f)   // top left
};

struct Sprite {
	vec4 uv;
	vec2 origin;
	uint image_index;
};

struct SpriteRenderable {
	vec4 color;
	uint sprite_index;
	float glitch;
};

layout (std430, binding=0) buffer sprite_buffer
{ 
	Sprite sprites[];
};

layout (std430, binding=1) buffer transform_buffer
{ 
	mat4 transforms[%SPRITE_RENDERABLE_CAPACITY%];
};

layout (std430, binding=2) buffer renderable_buffer
{ 
	SpriteRenderable renderables[%SPRITE_RENDERABLE_CAPACITY%];
};

layout (std430, binding=3) buffer draw_buffer
{ 
	uint draws[%SPRITE_RENDERABLE_CAPACITY%];
};

layout (binding=0) uniform view_proj
{
	mat4 view;
	mat4 proj;
};

out vec4 vert_color;
out vec2 vert_uv;
flat out uint sprite_image_index;

void main(void) {

	// Resove id's
	int instance_id = gl_VertexID /6;
	int vertex_id = gl_VertexID % 6;
	SpriteRenderable renderable = renderables[draws[instance_id]];
	Sprite sprite = sprites[renderable.sprite_index];
	mat4 transform = transforms[draws[instance_id]];
	vec2 source_uv = uvs[vertex_id];

	// I know there is a much better way to do this but its working right now so not a priority
	mat4 sprite_transform;
	sprite_transform[0] = vec4(sprite.uv.z, 0.0, 0.0, 0.0);
	sprite_transform[1] = vec4(0.0, sprite.uv.w, 0.0, 0.0);
	sprite_transform[2] = vec4(0.0, 0.0, 0.0, 0.0);
	sprite_transform[3] = vec4(0.0, 0.0, 0.0, 1.0);

	// Nullify any z value as the layer setup already dictates draw order
	transform[3].z = 0.0;

	vert_uv = vec2((sprite.uv.x / float(1024)) + (source_uv.x * (sprite.uv.z / float(1024))), 1.0 - ((sprite.uv.y / float(1024)) + (source_uv.y * (sprite.uv.w / float(1024)))));
	vert_color = renderable.color;
	sprite_image_index = sprite.image_index;
    gl_Position = proj * view * sprite_transform * transform * vec4(vertices[vertex_id], 0.0f, 1.0f);
}
)"