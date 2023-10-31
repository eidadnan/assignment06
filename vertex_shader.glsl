
#version 430 core

in vec4 b_position;
in vec2 b_texture_coord;
in vec4 b_color;

uniform mat4 u_transform;

out vec2 s_texture_coord;
out vec4 s_color;

void
main()
{
    s_texture_coord = b_texture_coord;
    gl_Position = u_transform * b_position;
    s_color = b_color;
}
