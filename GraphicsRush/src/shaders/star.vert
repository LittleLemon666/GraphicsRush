#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

uniform int star_time;
uniform float star_angle;
uniform int height;
uniform float r;

void main()
{
    float rotate_angle = star_time + star_angle;
    vec3 bias = vec3(r * cos(radians(rotate_angle)), star_time / 360.0, -r * sin(radians(rotate_angle)));
    gl_Position = u_projection * u_view * u_model * vec4(position + bias, 1.0f);
}