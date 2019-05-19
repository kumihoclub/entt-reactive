R"(
#version 430 core

in vec2 vert_uv;

out vec4 frag_color;

layout(location = 0) uniform sampler2D render_texture;

void main() {
	frag_color = texture(render_texture, vert_uv);
}  
)"