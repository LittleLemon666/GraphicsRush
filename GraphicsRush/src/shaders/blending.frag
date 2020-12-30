#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 texture_coordinate;
} f_in;

uniform sampler2D u_texture;

void main()
{    
    f_color = texture(u_texture, f_in.texture_coordinate);
}