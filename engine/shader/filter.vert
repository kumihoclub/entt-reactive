R"(
#version 430 core

vec2 vertices[6] = {
	// first triangle
	vec2(1.0f,  1.0f),  // top right
	vec2(1.0f, -1.0f),  // bottom right
	vec2(-1.0f,  1.0f),  // top left 
	// second triangle
	vec2(1.0f, -1.0f),  // bottom right
	vec2(-1.0f, -1.0f),  // bottom left
	vec2(-1.0f,  1.0f)   // top left
};

vec2 uvs[6] = {
	// first triangle
	vec2(1.0f, 1.0f),  // top right
	vec2(1.0f, 0.0f),  // bottom right
	vec2(0.0f, 1.0f),  // top left 
	// second triangle
	vec2(1.0f, 0.0f),  // bottom right
	vec2(0.0f, 0.0f),  // bottom left
	vec2(0.0f, 1.0f)   // top left
};

out vec2 vert_uv;

void main() {
	vert_uv = uvs[gl_VertexID];
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0); 
}  
)"