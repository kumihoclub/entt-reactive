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

struct Renderable {
	mat4 model;
	vec2 uvs[6];
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
	vert_color = renderables[stride_offset + (gl_VertexID /6)].color;
	vert_uv = renderables[stride_offset + (gl_VertexID /6)].uvs[gl_VertexID % 6];
    gl_Position = proj * view * renderables[stride_offset + gl_VertexID /6].model * vec4(vertices[gl_VertexID % 6], 0.0f, 1.0f);
	//gl_Position = vec4(vertices[gl_VertexID % 6], 0.0f, 1.0f);
}
)"