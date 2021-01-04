#version 430 core
out vec4 f_color;

uniform vec3 u_color;

uniform float time;
uniform float time_offset;

// range [0, 10.0) for up, range [10.0, 20.0) for bomb

void main()
{   
    vec4 color = vec4(u_color, (time < time_offset ? 1.0 : (1.0 - (time - time_offset) / 5.0)));
    //color = vec4(u_color, 0.5);
    f_color = color;
}