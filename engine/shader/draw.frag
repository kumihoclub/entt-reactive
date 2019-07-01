R"(
#version 430 core

in vec4 vert_color;
in vec2 vert_uv;
flat in uint sprite_image_index;

out vec4 frag_color;

layout(location = 1) uniform sampler2DArray sprite_texture;

void main()
{
	frag_color = texture(sprite_texture, vec3(vert_uv.xy, sprite_image_index));
	//frag_color = texture(sprite_texture, vert_uv.xy);
	//frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}
)"