#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec2 texture_coordinate;
} f_in;

uniform vec3 u_color_back;
uniform vec3 u_color_front;
uniform float offset;

void main()
{   
    vec3 color;
    if (f_in.texture_coordinate.x < offset)
        color = u_color_front;
    else
        color = u_color_back;
    f_color = vec4(color.rgb, 1.0f);
}