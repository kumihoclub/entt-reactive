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

/*vec2 uvs[6] = {
	// first triangle
	vec2(1.0f, 1.0f),  // top right
	vec2(1.0f, 0.0f),  // bottom right
	vec2(0.0f, 1.0f),  // top left 
	// second triangle
	vec2(1.0f, 0.0f),  // bottom right
	vec2(0.0f, 0.0f),  // bottom left
	vec2(0.0f, 1.0f)   // top left
};*/

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

struct Renderable {
	mat4 model;
	vec4 uv;
	vec4 color;
	float glitch;
};

out vec4 vert_color;
out vec2 vert_uv;

layout(location = 0) uniform sampler2D sprite_texture;
//uniform sampler2D glitch_texture;

layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;
layout(location = 3) uniform int stride_offset;

layout (std430, binding=0) buffer renderable
{ 
	Renderable renderables[];
};

void main(void) {

	// Resove id's
	int instance_id = gl_VertexID /6;
	int vertex_id = gl_VertexID % 6;

	// Resolve uv
	vec2 source_uv = uvs[vertex_id];
	vec4 renderable_uv = renderables[stride_offset + instance_id].uv;
	vert_uv = vec2(renderable_uv.x + (source_uv.x * renderable_uv.z), 1.0 - (renderable_uv.y + (source_uv.y * renderable_uv.w)));

	// Set color and pos
	vert_color = renderables[stride_offset + instance_id].color;
    gl_Position = proj * view * renderables[stride_offset + instance_id].model * vec4(vertices[vertex_id], 0.0f, 1.0f);
}
)"