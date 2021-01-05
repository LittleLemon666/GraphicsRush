#version 430 core
out vec4 f_color;

uniform vec3 u_color;

void main()
{   
    vec4 color = vec4(u_color, 0.7);
    f_color = color;
}