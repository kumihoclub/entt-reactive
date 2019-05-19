R"(
#version 430 core

in vec4 vert_color;
in vec2 vert_uv;

out vec4 frag_color;

layout(location = 0) uniform sampler2D sprite_texture;
//uniform sampler2D glitch_texture;

void main()
{
	//ivec2 sprite_texture_size = imageSize(sprite_texture);
    //frag_color = texture(sprite_texture, vec2(float(vert_uv.x) / float(sprite_texture_size.x), float(vert_uv.y) / float(sprite_texture_size.y));
	frag_color = texture(sprite_texture, vert_uv);
} 
)"