#version 430 core

out vec4 f_color;

in V_OUT
{
   vec2 texture_coordinate;
} f_in;

uniform sampler2D text;
uniform vec3 text_color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, f_in.texture_coordinate).r);
    f_color = vec4(text_color, 1.0) * sampled;
}