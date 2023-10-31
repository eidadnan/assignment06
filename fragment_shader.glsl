#version 430 core

out vec4 fColor;

in vec2 s_texture_coord;
in vec4 s_color;

uniform sampler2D  u_sampler;
uniform int u_surface_effect;
uniform vec4 u_diffuse_color;

void main()
{
    fColor = s_color;
}
